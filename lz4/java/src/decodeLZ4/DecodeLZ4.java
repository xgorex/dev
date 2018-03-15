package decodeLZ4;

public class DecodeLZ4 {

	private static InputBuffer		UnitIBuffer;
	private static int inputBufferWordDataSize;
	
	private static OutputBuffer		UnitOBuffer;

	
	
	private static void test_InputOutputBufferReadWrite(){
		
		System.out.println("[test_InputOutputBufferReadWrite] Running test...");
		
		inputBufferWordDataSize = 64;
		String inputBufferData = "input.file"; 
		UnitIBuffer = new InputBuffer(inputBufferWordDataSize,inputBufferData);
	
		
		int outputBufferWordDataSize = 64;
		String outputBufferData = "output.file"; 
		UnitOBuffer = new OutputBuffer(outputBufferWordDataSize,outputBufferData);		
		
		do{
			UnitIBuffer.hwIBufferReadStrobe();
			
			int bytesToRead = UnitIBuffer.hwIBufferReadGetMask() == 0 ? inputBufferWordDataSize : UnitIBuffer.hwIBufferReadGetMask();
			//System.out.print("[");
			for(int i=0; i<bytesToRead;i++){
				//System.out.printf("%c",ibuff.hwIBufferReadGetData()[i]);
			}
			//System.out.print("]"+"\n");
			
			UnitOBuffer.hwOBufferWriteSetData(UnitIBuffer.hwIBufferReadGetData());
			UnitOBuffer.hwOBufferWriteSetMask(UnitIBuffer.hwIBufferReadGetMask());
			UnitOBuffer.hwOBufferWriteSetEOB(UnitIBuffer.hwIBufferReadGetEOB());
			UnitOBuffer.hwOBufferWriteStrobe();
		}while(!UnitIBuffer.hwIBufferReadGetEOB());
		
		UnitIBuffer.ibuff.systemIBufferClose();
		UnitOBuffer.obuff.systemOBufferClose();		
		
		//check both buffers: diff
			byte pass=1;
			InputBuffer in1 = new InputBuffer(1,"input.file");
			InputBuffer in2 = new InputBuffer(1, "output.file");
			
			do{
				in1.hwIBufferReadStrobe();
				in2.hwIBufferReadStrobe();
				if(in1.hwIBufferReadGetData()[0] != in2.hwIBufferReadGetData()[0]){
					pass=0;
					break;
				}
				
			}while(!in1.hwIBufferReadGetEOB());
			
			if(pass ==1)
				System.out.println("[test_InputOutputBufferWrireRead] PASSED.");
			else
				System.out.println("[test_InputOutputBufferWrireRead] FAILED.");
			
			in1.ibuff.systemIBufferClose();
			in2.ibuff.systemIBufferClose();
		
	}
	
	
	public static void main(String[] args){
		System.out.print("@INF: Pozdrowienia z DecodeLZ4\n");
		
		test_InputOutputBufferReadWrite();
		
		HistoryBuffer hbuff = new HistoryBuffer(96);
		for(int i=0; i<100;i++){
			hbuff.hBufferWriteWord();
		}
		
		for(int i=0; i<15;i++){
			hbuff.hBufferReadWord();
		}
		
	}
}
