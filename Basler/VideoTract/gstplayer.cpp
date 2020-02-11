#include "gstplayer.h"

GstPlayer::GstPlayer(OneNode _fullServer, OneNode _halfServer, QObject *parent):
    QObject(parent),
    sampleCount(0),
    fullServer(_fullServer),
    halfServer(_halfServer),
    width(VIDEO_WIDTH),
    height(VIDEO_HEIGHT),
    pipelineReady(false),
    first(true)
{
    GError* err;
    if (!gst_init_check (nullptr, nullptr, &err))
    {
        emit Message(LoggerMsg::Fatal,QString("Инициализация GStreamer с ошибкой: %s").arg(QString(err->message)));
        exit(EXIT_FAILURE);
    }
#ifdef Debug_Config
    gst_debug_set_active(true);
#endif
}

bool GstPlayer::play()
{
    if (!pipelineReady) return false;
    GstStateChangeReturn ret=gst_element_set_state (Pipeline, GST_STATE_PLAYING);
#ifdef Debug_Config
    GST_DEBUG_BIN_TO_DOT_FILE(reinterpret_cast<GstBin*>(Pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "OLIKpipeline");
    emit Message(LoggerMsg::Info,"Gstreamer play start");
#endif
    if (ret==GST_STATE_CHANGE_FAILURE)
    {
        emit Message(LoggerMsg::Warning,"The Pipeline state change failed");
        return false;
    }
    return true;
}


//gst-launch-1.0 filesrc location=/home/olikke/1.mp4  ! decodebin ! tee name=VideoTee \
//    VideoTee. ! queue ! jpegenc quality=85 ! rtpjpegpay ! udpsink host=224.1.1.1 auto-multicast=true port=5000

/*gst-launch-1.0 filesrc location=/home/olikke/1.mp4  ! decodebin ! tee name=VideoTee \
    VideoTee. ! queue ! autovideosink \
    VideoTee. ! queue ! jpegenc quality=85 ! rtpjpegpay ! udpsink host=224.1.1.1 auto-multicast=true port=5000 \
    VideoTee. ! queue ! jpegenc quality=25 ! rtpjpegpay ! udpsink host=224.1.1.1 auto-multicast=true port=5002*/

/*AppSrc = reinterpret_cast<GstAppSrc*>(gst_bin_get_by_name(GST_BIN(Pipeline), "mappsrc"));*/

//gst-launch-1.0 filesrc location=/home/olikke/1.mp4  ! decodebin ! tee name=VideoTee \
VideoTee. ! queue ! xvimagesink \
VideoTee. ! queue ! jpegenc quality=85 ! rtpjpegpay ! multiudpsink clients=224.1.1.1:5000 multicast-iface=192.168.3.12:5000 \
VideoTee. ! queue ! jpegenc quality=25 ! rtpjpegpay ! multiudpsink clients=224.1.1.1:5002 multicast-iface=192.168.3.12:5002

//gst-launch-1.0 udpsrc multicast-group=224.1.1.5 auto-multicast=true port=5000 caps='application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)JPEG, payload=(int)26' ! rtpjpegdepay ! jpegdec ! autovideosink

/*gst-launch-1.0 udpsrc multicast-group=224.1.1.1 auto-multicast=true port=5000 caps='application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)JPEG, payload=(int)26' ! rtpjpegdepay ! jpegdec ! autovideosink*/

GstElement* GstPlayer::createElement(const gchar *name)
{
    GstElement* Element = gst_element_factory_make(name, nullptr);
    if (!Element)
    {
        emit Message(LoggerMsg::Warning,QString("Failed to create %s").arg(QString(name)));
        return nullptr;
    }
    return Element;
}

bool GstPlayer::prepare()
{
   Pipeline = gst_pipeline_new("VideoRtpServer");        
    if (!Pipeline)
    {
        emit Message(LoggerMsg::Warning,"Failed to create Gst Pipeline");
        return false;
    }

    AppSrc=reinterpret_cast<GstAppSrc*>(createElement("appsrc"));
    Tee=createElement("tee");
    VideoScale=createElement("videoscale");
    CapsHalf=createElement("capsfilter");
    QueueFull=createElement("queue");
    QueueHalf=createElement("queue");
    UdpSinkFull=createElement("multiudpsink");
    JpegEncFull=createElement("jpegenc");
    RtpJpegPayFull=createElement("rtpjpegpay");
    UdpSinkHalf=createElement("multiudpsink");
    JpegEncHalf=createElement("jpegenc");
    RtpJpegPayHalf=createElement("rtpjpegpay");
    VideoConvert=createElement("videoconvert");


    if (!AppSrc || !Tee || !QueueFull || !UdpSinkFull || !JpegEncFull || !RtpJpegPayFull || !VideoScale || !CapsHalf || !UdpSinkHalf || !JpegEncHalf || !RtpJpegPayHalf || !QueueHalf )
    {
        emit Message(LoggerMsg::Warning,"Failed to create Gst Elements");
        return false;
    }

    g_object_set (reinterpret_cast<GObject*> (CapsHalf), "caps",
    gst_caps_new_simple ("video/x-raw",
                         "format", G_TYPE_STRING, "I420",
                         "width", G_TYPE_INT, static_cast<int>(width/2),
                         "height", G_TYPE_INT, static_cast<int>(height/2),
                         "framerate", GST_TYPE_FRACTION, 0, 1,
                         NULL), NULL);
    QString temp;
    if (isColorCamera) temp="UYVY"; else temp="GRAY8";

    g_object_set (reinterpret_cast<GObject*> (AppSrc), "caps",
    gst_caps_new_simple ("video/x-raw",
                         "format", G_TYPE_STRING,temp.toLocal8Bit().constData(),
                         "width", G_TYPE_INT, width,
                         "height", G_TYPE_INT, height,
                         "framerate", GST_TYPE_FRACTION, 0, 1,
                         NULL), NULL);

        g_object_set (G_OBJECT (AppSrc),
              "stream-type", 0, // GST_APP_STREAM_TYPE_STREAM
              "format", GST_FORMAT_TIME,
          "is-live", TRUE,
          NULL);



    QString temp1=fullServer.sIP+QString(':')+fullServer.Port;
//    QString temp2=fullServer.oIP+QString(':')+fullServer.Port;
    g_object_set(reinterpret_cast<GObject*>(UdpSinkFull), "clients",temp1.toLocal8Bit().constData(),
                 "multicast-iface","eth0",/*temp2.toLocal8Bit().constData()*,"auto-multicast",true,"send-duplicates",false,*/
                 nullptr);
    g_object_set(reinterpret_cast<GObject*>(JpegEncFull), "quality",fullServer.quality, nullptr);

    temp1=halfServer.sIP+QString(':')+halfServer.Port;
//    temp2=halfServer.oIP+QString(':')+halfServer.Port;
    g_object_set(reinterpret_cast<GObject*>(UdpSinkHalf), "clients", temp1.toLocal8Bit().constData(),
                 "multicast-iface","eth0",/*temp2.toLocal8Bit().constData(),"auto-multicast",true,"send-duplicates",false,*/
                  nullptr);
    g_object_set(reinterpret_cast<GObject*>(JpegEncHalf), "quality", halfServer.quality, nullptr);

    gst_bin_add_many (reinterpret_cast<GstBin*> (Pipeline), reinterpret_cast<GstElement*>(AppSrc), VideoConvert, Tee,
                       QueueFull, JpegEncFull, RtpJpegPayFull, UdpSinkFull,
                       QueueHalf, VideoScale, CapsHalf, JpegEncHalf, RtpJpegPayHalf, UdpSinkHalf,nullptr);


    if (!gst_element_link_many (reinterpret_cast<GstElement*>(AppSrc),VideoConvert,Tee,nullptr))
    {
        return false ;
    }
    if (!gst_element_link_many(Tee,QueueFull, JpegEncFull, RtpJpegPayFull, UdpSinkFull, nullptr))
    {
        return false;
    }
    if (!gst_element_link_many(Tee, QueueHalf, VideoScale, CapsHalf, JpegEncHalf, RtpJpegPayHalf, UdpSinkHalf, nullptr))
    {
        return false;
    }
   pipelineReady=true;
        qWarning("all looking good");
    return true;
}

void GstPlayer::stop()
{
#ifdef Debug_Config
    qDebug("Create Stop pipeline");
    GST_DEBUG_BIN_TO_DOT_FILE(reinterpret_cast<GstBin*>(Pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "OLIKKEPIPELINE");
#endif
    gst_element_set_state (Pipeline, GST_STATE_NULL);
   if (Pipeline) gst_object_unref(reinterpret_cast<GstObject*>(Pipeline));
   pipelineReady=false;
}

void GstPlayer::changeSize(int _width, int _height, bool _colorCamera)
{
    if (width!=_width || height!=_height || isColorCamera!=_colorCamera)
    {
           width=_width;
           height=_height;
           stop();
           isColorCamera=_colorCamera;
           prepare();
           play();
    }
}

void GstPlayer::pushBufferInGst(GstBuffer *buffer)
{
    if (!Pipeline) return;
    GstFlowReturn ret;

        //olikke
        /*
         * крч тут проблемы с согласованием скорости входного(no const) потока от источника данных и потока gstreamer RTPServer
         * если скорость gstreamer<Basler, то получаем задержку в видео + gstreamer копит кадры до полного заполнения доступной памяти
         * (формально это не утечка, методы appsrc че то как то не контролируют эту ситуацию)
         * поэтому возьмем, и не будем вообще ничего ставить
         * т.е. так быстро, как только можно ASAP*/
    static GstClockTime timestamp = 0;
    GST_BUFFER_PTS (buffer) = timestamp;
    GST_BUFFER_DURATION (buffer) =gst_util_uint64_scale_int (1, GST_SECOND, 40);
    timestamp += GST_BUFFER_DURATION (buffer);

    g_signal_emit_by_name (AppSrc, "push-buffer", buffer, &ret);
    gst_buffer_unref (buffer);

    if (ret != GST_FLOW_OK)
    {

        qWarning("Push-buffer error\n");
    }
}

void GstPlayer::getStatistic(quint64 & FullServerTotalBytes, quint64 & HalfServerTotalBytes, quint64 &TotalSampleCount)
{    
    GstStructure* data;
    g_signal_emit_by_name(reinterpret_cast<GObject*>(UdpSinkFull),"get-stats",fullServer.sIP.toLocal8Bit().data(),fullServer.Port.toInt(),&data);
 //   multiudpsink-stats, bytes-sent=(guint64)0, packets-sent=(guint64)0, connect-time=(guint64)1554384213913968000, disconnect-time=(guint64)0;
//    qDebug(gst_structure_to_string(data));
    unsigned long temp;
    gst_structure_get_uint64(data,"bytes-sent",&temp);
    FullServerTotalBytes=static_cast<quint64>(temp);
    g_signal_emit_by_name(reinterpret_cast<GObject*>(UdpSinkHalf),"get-stats",halfServer.sIP.toLocal8Bit().data(),halfServer.Port.toInt(),&data);

    gst_structure_get_uint64(data,"bytes-sent",&temp);
    HalfServerTotalBytes=static_cast<quint64>(temp);
    TotalSampleCount=sampleCount;
}

void GstPlayer::newSample()
{    
   if (first)
   {
       play();
       first=false;
   }
   sampleCount++;
   SampleQueue* queue=reinterpret_cast<SampleQueue*>(sender());
   if (queue->CountQueue()>0)
    {
        GstBuffer* sample=queue->dequeue();
        pushBufferInGst(sample);
   }
}



