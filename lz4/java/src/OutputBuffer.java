package decodeLZ4;

public class OutputBuffer {

	SystemService obuff;
	private int 	hwOBufferWordSize;
	private byte[]	hwOBufferWriteData;
	private byte	hwOBufferWriteMask;
	private boolean	hwOBufferWriteEOB;	 
	
	
	public void hwOBufferWriteSetData(byte[] data){
		hwOBufferWriteData = data;
	}
	
	public void hwOBufferWriteSetMask(byte mask){
		hwOBufferWriteMask = mask;
	}
	
	public void hwOBufferWriteSetEOB(boolean eob){
		hwOBufferWriteEOB = eob;
	}	
	
	public int hwOBufferWriteStrobe(){
		
		int bytesToWrite = (hwOBufferWriteMask == 0) ? hwOBufferWordSize : hwOBufferWriteMask ;
	
		//System.out.println("Write");
		for(int i=0; i<bytesToWrite;i++){
			obuff.write(hwOBufferWriteData[i]);
			//System.out.printf("%c",hwOBufferWriteData[i]);
		}
		return 1;
	}
	
	//Konstruktor
	public OutputBuffer(int hwBufferInterfaceDataSize, String outputFileName){
		obuff = new SystemService();
		obuff.systemOBufferOpen(outputFileName);

		hwOBufferWordSize = hwBufferInterfaceDataSize;
		
		System.out.println("@INF: Utworzony Output Buffer Unit: ["+hwBufferInterfaceDataSize+"]["+outputFileName+"]");		
	}
	
	//Destruktor
	protected void finalize(){
		obuff.systemOBufferClose();
	}
}
