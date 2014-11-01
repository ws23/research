package test;

import java.net.InetSocketAddress;
import java.net.Socket;
import java.io.BufferedOutputStream;

public class client {
    private String address = &quot;127.0.0.1&quot;;// 連線的ip
    private int port = 8765;// 連線的port

    public SocketClient() {

        Socket client = new Socket();
        InetSocketAddress isa = new InetSocketAddress(this.address, this.port);
        try {
            client.connect(isa, 10000);
            BufferedOutputStream out = new BufferedOutputStream(client
                    .getOutputStream());
            // 送出字串
            out.write(&quot;Send From Client &quot;.getBytes());
            out.flush();
            out.close();
            out = null;
            client.close();
            client = null;

        } catch (java.io.IOException e) {
            System.out.println(&quot;Socket連線有問題 !&quot;);
            System.out.println(&quot;IOException :&quot; + e.toString());
        }
    }

    public static void main(String args[]) {
        new SocketClient();
    }
}
