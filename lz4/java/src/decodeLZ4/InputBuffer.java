package decodeLZ4;

public class InputBuffer {
	
	//Input System Memory Buffer <== 
	//	The Input FILE will be pushed here 
	SystemService ibuff;
	
	//Hardware buffer interface data size
	private	int 	hwIBufferWordSize;
	private byte[]	hwIBufferReadData;
	private byte	hwIBufferReadMask;
	private boolean	hwIBufferReadEOB;	
 
	public byte[]	hwIBufferReadGetData(){
		return this.hwIBufferReadData;
	};
	public byte		hwIBufferReadGetMask(){
		return this.hwIBufferReadMask;
	};

	public boolean		hwIBufferReadGetEOB(){
		return this.hwIBufferReadEOB;
	};	
	
	//private InputBufferOutput InputBufferReadBytes8(BufferedReader inputBuffer){
	public void hwIBufferReadStrobe(){
		byte[] data = new byte[this.hwIBufferWordSize];
		byte mask=0;
		boolean eof=false;
		byte znak=0;

			for(int i=0; i<this.hwIBufferWordSize; i++) {
				 if( (znak = (byte)this.ibuff.read()) != -1){
					 data[i]=znak;
				 }else{
					 mask = (byte)i;
					 eof= true;
					 break;
				 }
			}
			//return new InputBufferInterface(data,mask);
			hwIBufferReadData=data;
			hwIBufferReadMask=mask;
			hwIBufferReadEOB=eof;
	}
	
	
	//Konstruktor
	public InputBuffer(int hwBufferInterfaceDataSize,String inputFileName){

		ibuff = new SystemService();
		ibuff.systemIBufferOpen(inputFileName);		
		
		hwIBufferWordSize = hwBufferInterfaceDataSize;
		hwIBufferReadData = new byte[hwIBufferWordSize];
		
		System.out.println("@INF: Utworzony Input Buffer Unit: ["+hwBufferInterfaceDataSize+"]["+inputFileName+"]");
		
	};
	
	//Destruktor
	protected void finalise(){
		ibuff.systemIBufferClose();
	}
	
}
