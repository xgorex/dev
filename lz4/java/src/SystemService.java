package decodeLZ4;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.FileInputStream;
import java.io.BufferedInputStream;
import java.io.FileOutputStream;
import java.io.BufferedOutputStream;

public class SystemService {

	private BufferedInputStream systemIBuffer;
	private BufferedOutputStream systemOBuffer;
	
	
	public int read(){
		try{
			return this.systemIBuffer.read();
		}catch (IOException e){
			e.printStackTrace();
			return -1;
		}
	}
	
	public int write(int inchar){
		try{
			this.systemOBuffer.write(inchar);
			return 1;
		}catch (IOException e){
			e.printStackTrace();
			return -1;
		}
	}	
	
	
	public int systemIBufferOpen(String instream){
		
		try{
			this.systemIBuffer = new BufferedInputStream(new FileInputStream(instream));
		}catch(IOException e){
			e.printStackTrace();
			return 0;
		}
		return 1;
	}

	public void systemIBufferClose(){
		try{
			this.systemIBuffer.close();
		}catch(IOException e){
			e.printStackTrace();
		}
	}
	
	
	public int systemOBufferOpen(String outstream){
		try{
			this.systemOBuffer = new BufferedOutputStream(new FileOutputStream(outstream));
			//BufferedOutputStream file = new BufferedOutputStream(new FileOutputStream(outstream));
			return 1;
		}catch (IOException e){
			e.printStackTrace();
			return 0;
		}
		
	}
	
	public void systemOBufferClose(){
		try{
			this.systemOBuffer.close();
		}catch(IOException e){
			e.printStackTrace();
		}
	}	
	
	
	
}
