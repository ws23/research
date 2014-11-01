import java.io.*;
import java.net.*;
public class Server{
	public static void main	(String[] args) throws IOException{
		ServerSocket server = new ServerSocket(5678);
		Socket client = server.accept();
		BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream()));
		PrintWriter out = new PrintWriter(client.getOutputStream());
		while(true){
			String str = in.readLine();
			System.out.println(str);
			out.println("has receive...");
			out.flush();
			if(str.equals("end"))
				break;
		}
		client.close();
	}
}
