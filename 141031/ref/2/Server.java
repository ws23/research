import java.io.*;
import java.net.*;

public class Server {
    public static void main(String[] args) { 
        try { 
            int port = Integer.parseInt(args[0]);
            System.out.println("簡易檔案接收..."); 
            System.out.printf("將接收檔案於連接埠: %d%n", port); 

            ServerSocket serverSkt = new ServerSocket(port); 
            
            while(true) { 
                System.out.println("傾聽中...."); 
                
                Socket clientSkt = serverSkt.accept();
                
                System.out.printf("與 %s 建立連線%n", 
                        clientSkt.getInetAddress().toString());  
                
                // 取得檔案名稱
                String fileName = new BufferedReader(
                                    new InputStreamReader(
                                      clientSkt.getInputStream())).readLine();
                
                System.out.printf("接收檔案 %s ...", fileName); 

                BufferedInputStream inputStream = 
                    new BufferedInputStream(clientSkt.getInputStream()); 
                BufferedOutputStream outputStream = 
                    new BufferedOutputStream(new FileOutputStream(fileName)); 
                
                int readin; 
                while((readin = inputStream.read()) != -1) { 
                    outputStream.write(readin);
                    Thread.yield();
                } 

                outputStream.flush();
                outputStream.close();                
                inputStream.close(); 
                
                clientSkt.close(); 
                
                System.out.println("\n檔案接收完畢！"); 
            } 
        } 
        catch(Exception e) { 
            e.printStackTrace(); 
        } 
    }     
}
