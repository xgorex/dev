package decodeLZ4;

public class HistoryBuffer {

	private static int hbMargin=32;
	private static int hbActiveBSize = 64;				//rozmiar bufora historii
	private static int hbSize = hbActiveBSize + hbMargin;	//rozmiar rzeczywisty bufora,	
	private static int hbActiveBHead=0;				//wskaznik zapisu danych, opiera sie na wskaznikach BSIZE oraz HBFLUSH
	private static int hbActiveBTail=0;				//okresla dolna granice bufora historii HBHEAD - HBTAIL == HBSIZE,
												//jest to HEAD dla HBFLUSH
	private static int hbStoreTail=0;				//dane sfluszowane do OBUFF-a
	private byte[] hBuffer;
	
	
	
	
	
	private void hActiveBTailUpdate(){
		byte hGRt=0;
		
		if(hbActiveBHead > hbActiveBTail){
			hGRt = 1;
		}
		
		if(hGRt==1){
			if(hbActiveBHead > hbActiveBSize){
				hbActiveBTail = hbActiveBHead - hbActiveBSize;
			}
		}else{
			if (((hbSize - hbActiveBTail) + hbActiveBHead) > hbActiveBSize){
				
				int step = hbActiveBSize - ((hbSize - hbActiveBTail) + hbActiveBHead);
				if((hbActiveBTail + step) >hbSize){
					step = step - (hbSize - hbActiveBTail);
				}
				hbActiveBTail = step;
			}
			 
		}
	}
	
	
	private boolean hBufferFullCheck(){
		boolean full=false;
		
		System.out.print("[BufferFullCheck] HEAD="+hbActiveBHead+" Flush tail="+hbStoreTail + ":" );
		if(((hbActiveBHead + 1) == hbStoreTail) || (hbActiveBHead==hbSize-1 && hbStoreTail ==0 ))   {
			full = true;
			System.out.println("FULL");
		}else{
			System.out.println("NOT FULL");
		}
		
		return full;
	}
	
	private boolean hBufferEmptyCheck(){
		boolean empty = false;
		
		System.out.print("[BufferEMptyCheck] HEAD="+hbActiveBTail+" TAIL="+hbStoreTail + ":");
		if(hbStoreTail == hbActiveBTail){
			empty = true;
			System.out.println("EMPTY");
		}else{
			System.out.println("NOT EMPTY");
		}
		return empty;
	}
	
	public void hBufferReadWord(){
		byte data = 0;
		byte headbigger=0;

		if(hbActiveBTail > hbStoreTail){
			headbigger=1;
		}
		
		//BufferEmptyCheck determinuje czy mozliwe sa dalsze odczyty
		//sprawdz warunki Empty
		if(!hBufferEmptyCheck() ){
			if(headbigger == 1){
				hbStoreTail++;
				data = hBuffer[hbStoreTail];
				System.out.println("[BufferRead] Read @"+hbStoreTail);
				
			}else{
				if(hbStoreTail < hbSize-1){
					hbStoreTail++;
				}else{
					hbStoreTail=0;
				}
				data = hBuffer[hbStoreTail];
				System.out.println("[BufferRead] Read @"+hbStoreTail);
			}
		}
	
	}
	
	public void hBufferWriteWord(){
		byte data=0;
		byte headbigger=0;
		
		if(hbActiveBHead >= hbStoreTail){			//tail moze dogonic head, ale head nie dogoni tail. head idac od tylu zatrzyma sie jeden wpis ponizej.
			headbigger=1;
		}else{
			headbigger=0;
		}
		
		//BufferFullCheck determinuje czy mozliwy jest dalszy zapis
		//sprawdz warunki zapisu
		if( !hBufferFullCheck() ){
			if(headbigger == 1){
	
				if(hbActiveBHead < hbSize-1){
					hbActiveBHead++;
				}else{
					if(hbStoreTail != 0){
						hbActiveBHead=0;
					}
				}
				hBuffer[hbActiveBHead] = data;
				System.out.println("HB# Wrote @"+hbActiveBHead);
					
			}else{
				if(hbActiveBHead < hbStoreTail) {
					hbActiveBHead++;
				}				
				System.out.println("HB# Wrote @"+hbActiveBHead);
				hBuffer[hbActiveBHead] = data;
			}
			
		}
		hActiveBTailUpdate();
	}
	
	
	
	
	
	
	
	public HistoryBuffer(int bufferSize){
		this.hbActiveBSize = bufferSize;
		this.hbMargin = bufferSize / 5;
		this.hbSize = bufferSize + hbMargin;
		this.hBuffer = new byte[hbSize];
		System.out.println("@INF: Utworzony: HistoryBuffer; activeBuffer size="+hbActiveBSize+", buffer size="+hbSize);
	};
	
}
