import javax.media.*;
import javax.media.control.*;
import javax.media.protocol.*;
import javax.media.format.*;
import java.io.IOException;
import java.io.File;
public class MediaTransmitter {
    private MediaLocator mediaLocator = null;
    private DataSink dataSink = null;
    private Processor mediaProcessor = null;
    private static final Format[] FORMATS = new Format[] {
        new AudioFormat(AudioFormat.MPEG_RTP)};
    private static final ContentDescriptor CONTENT_DESCRIPTOR =         
                    new ContentDescriptor(ContentDescriptor.RAW_RTP);
    public MediaTransmitter(MediaLocator locator) {
        mediaLocator = locator;
    }
// 開始傳送media
    public void startTransmitting() throws IOException {
// 啟動processor
        mediaProcessor.start();
        dataSink.open();
        dataSink.start();
    }
// 停止傳送media
    public void stopTransmitting() throws IOException {
        dataSink.stop();
        mediaProcessor.stop();
        mediaProcessor.close();
    }
    public void setDataSource(DataSource ds) throws IOException, 
        NoProcessorException, CannotRealizeException, NoDataSinkException {
        mediaProcessor = Manager.createRealizedProcessor(
            new ProcessorModel(ds, FORMATS, CONTENT_DESCRIPTOR));
        dataSink = Manager.createDataSink(mediaProcessor.getDataOutput(), 
                                          mediaLocator);
    }
    public static void printUsage() {
        System.out.println("使用方式 : java MediaTransmitter mediaLocator " +
                           "mediaFile");
        System.out.println("  例如 : java MediaTransmitter " +
            "rtp://192.192.50.21:4800/audio music.mp3");
    }
    public static void main(String[] args) {
        try {
            if (args.length == 2) {
                MediaLocator locator = new MediaLocator(args[0]);
                MediaTransmitter transmitter = new MediaTransmitter(locator);
                System.out.println("建立media locator: '" + 
                                   locator + "'");
                File mediaFile = new File(args[1]);
                DataSource source = Manager.createDataSource(
                    new MediaLocator(mediaFile.toURL()));
                System.out.println("建立資料源 : '" + 
                                   mediaFile.getAbsolutePath() + "'");
                transmitter.setDataSource(source);
                System.out.println("設定transmitter的資料源");
                transmitter.startTransmitting();
                System.out.println("開始傳送 ...");
                System.out.println(" 按<Enter>鍵離開 ");
                System.in.read();
                transmitter.stopTransmitting();
            } else {
                printUsage();
            }
        } catch (Throwable t) {
            t.printStackTrace();
        }
        System.exit(0);
    }
}
