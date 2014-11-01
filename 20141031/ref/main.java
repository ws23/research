package test;

public class main {

    public static void main(String args[])
    {
        if(args.length==0)
            System.out.println(&quot;請傳入參數server或client&quot;);
        if(args[0].equals(&quot;server&quot;))
            (new SocketServer()).start();
        else
            new SocketClient();
    }
}
