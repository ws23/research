package test;

import java.net.ServerSocket;
import java.net.Socket;

public class server extends java.lang.Thread {

    private boolean OutServer = false;
    private ServerSocket server;
    private final int ServerPort = 8765;// 要監控的port

    public SocketServer() {
        try {
            server = new ServerSocket(ServerPort);

        } catch (java.io.IOException e) {
            System.out.println(&quot;Socket啟動有問題 !&quot;);
            System.out.println(&quot;IOException :&quot; + e.toString());
        }
    }

    public void run() {
        Socket socket;
        java.io.BufferedInputStream in;

        System.out.println(&quot;伺服器已啟動 !&quot;);
        while (!OutServer) {
            socket = null;
            try {
                synchronized (server) {
                    socket = server.accept();
                }
                System.out.println(&quot;取得連線 : InetAddress = &quot;
                        + socket.getInetAddress());
                // TimeOut時間
                socket.setSoTimeout(15000);

                in = new java.io.BufferedInputStream(socket.getInputStream());
                byte[] b = new byte[1024];
                String data = &quot;&quot;;
                int length;
                while ((length = in.read(b)) &gt; 0)// &lt;=0的話就是結束了
                {
                    data += new String(b, 0, length);
                }

                System.out.println(&quot;我取得的值:&quot; + data);
                in.close();
                in = null;
                socket.close();

            } catch (java.io.IOException e) {
                System.out.println(&quot;Socket連線有問題 !&quot;);
                System.out.println(&quot;IOException :&quot; + e.toString());
            }

        }
    }

    public static void main(String args[]) {
        (new SocketServer()).start();
    }

}
