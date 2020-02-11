#include "gstplayer.h"

GstPlayer::GstPlayer(QObject *parent) : QObject(parent), NeedToInitialize(false)
{
    GError* err;
    if (!gst_init_check (nullptr, nullptr, &err))
    {
        qCritical("gst_init Failed %s\n", err->message);
        exit(EXIT_FAILURE);
    }
}

void GstPlayer::getFrameRate(unsigned long &sampleCount, double &FrameRate)
{
    sampleCount=SampleCount;
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = finish - start;
    start=finish;
    FrameRate=(SampleCount-RememberCount)/elapsed.count()*1000;
    RememberCount=SampleCount;
}

void GstPlayer::Prepare(QString multicastIP, int multicastPort)
{
    MulticastIP=multicastIP;
    MulticastPort=multicastPort;
    if (prepare())  gst_element_set_state (Pipeline, GST_STATE_PLAYING);
}

void GstPlayer::Stop()
{
    gst_element_set_state (Pipeline, GST_STATE_NULL);
    GST_DEBUG_BIN_TO_DOT_FILE(reinterpret_cast<GstBin*>(Pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "BaslerSimpleClient GstPlayer Stop");
    if (Pipeline) gst_object_unref (GST_OBJECT(Pipeline));
}

/*gst-launch-1.0 udpsrc multicast-group=224.1.1.5 auto-multicast=true port=5000 caps='application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)JPEG, payload=(int)26' ! rtpjpegdepay ! jpegdec ! autovideosink*/

GstElement* GstPlayer::createElement(const gchar *name)
{
    GstElement* Element = gst_element_factory_make(name, nullptr);
    if (!Element)
    {
        qDebug("Failed to create %s",name);
        return nullptr;
    }
    return Element;
}

GstFlowReturn new_preroll(GstAppSink *appsink, gpointer data)
{
  Q_UNUSED (appsink)
  Q_UNUSED (data)
  return GST_FLOW_OK;
}

GstFlowReturn new_sample(GstAppSink *appsink, gpointer data)
{
  GstPlayer* ap=reinterpret_cast<GstPlayer*>(data);
  GstSample *sample = gst_app_sink_pull_sample(appsink);
  //получили буфер
  GstBuffer *buffer = gst_sample_get_buffer(sample);
  //скопировали его себе
  GstBuffer *appbuffer =  gst_buffer_copy(buffer);
  if (ap->NeedToInitialize)
  {
      GstCaps *caps = gst_sample_get_caps (sample);
      if (!caps) return GST_FLOW_OK;
      GstStructure *info =gst_caps_get_structure (caps, 0);
      QSize size;
      gst_structure_get_int(info,"width",&size.rwidth());
      gst_structure_get_int(info,"height",&size.rheight());
      ap->SetSize(size);
      ap->RememberCount=0;
      ap->start=std::chrono::high_resolution_clock::now();
  }
  //освободили память
  gst_sample_unref(sample);
  if (!ap->mSampleQueue)
  {
      g_print("new_sample ap->mSampleQueue\n");
      return GST_FLOW_OK;
  }
  GstMapInfo map;
  gst_buffer_map (appbuffer, &map, GST_MAP_READ);
  QImage img= QImage(map.data, ap->Size.width(),ap->Size.height(),QImage::Format_RGB32);
  gst_buffer_unmap (appbuffer, &map);
  gst_buffer_unref(appbuffer);
  auto ptr =std::make_shared<QImage>(img);
  ap->mSampleQueue->Enqueue(ptr);
  ap->SampleCount++;
  return GST_FLOW_OK;
}

void GstPlayer::setSampleQueue(sampleQueue* _sampleQueue)
{
    mSampleQueue=_sampleQueue;
}

void GstPlayer::SetSize(QSize sz)
{
    Size=sz;
    NeedToInitialize=false;
    SampleCount=0;
    emit GetSize(sz);
}

bool GstPlayer::prepare()
{
    Pipeline = gst_pipeline_new("VideoRtpServer");
    NeedToInitialize=true;
    if (!Pipeline)
    {
        qCritical("Failed to create Gst Pipeline");
        return false;
    }

    UdpSrc =createElement("udpsrc");
    RtpJpegDepay=createElement("rtpjpegdepay");
    JpegDec=createElement("jpegdec");
    AppSink=createElement("appsink");
    Convert=createElement("videoconvert");

    if (!UdpSrc  || !RtpJpegDepay || !JpegDec|| !AppSink || !Convert)
    {
        qCritical("Something wrong. Gst with love ;)");
        return false;
    }

    GstCaps *caps = gst_caps_from_string ("application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)JPEG, payload=(int)26");
    g_object_set(reinterpret_cast<GObject*>(UdpSrc), "multicast-group",MulticastIP.toLocal8Bit().constData(),"auto-multicast",true,"port",MulticastPort,"caps",caps,nullptr);
    gst_caps_unref(caps);

    caps = gst_caps_from_string ("video/x-raw, format=BGRx");
    gst_app_sink_set_caps (reinterpret_cast<GstAppSink*>(AppSink), caps);
    gst_caps_unref(caps);

    gst_app_sink_set_emit_signals(reinterpret_cast<GstAppSink*>(AppSink),true);
    gst_app_sink_set_drop(reinterpret_cast<GstAppSink*>(AppSink), true);
    gst_app_sink_set_max_buffers(reinterpret_cast<GstAppSink*>(AppSink), 1);
    GstAppSinkCallbacks callbacks = { NULL, new_preroll, new_sample };
    gst_app_sink_set_callbacks (GST_APP_SINK(AppSink), &callbacks, this, NULL);


    gst_bin_add_many (reinterpret_cast<GstBin*> (Pipeline), UdpSrc,RtpJpegDepay,JpegDec,AppSink,Convert,nullptr);

      if (!gst_element_link_many(UdpSrc,RtpJpegDepay,JpegDec,Convert,AppSink,nullptr))
      {
          qCritical("Failed to link gst elements. oops");
          return false;
       }
        return true;
}
