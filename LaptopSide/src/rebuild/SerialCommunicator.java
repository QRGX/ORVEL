package rebuild;

/** Notes on Class:
 * Based on the code from the old Java Swing version of the ground station
 * Uses a serial library that (should) work for all operating systems and doesn't have any weird dependencies (to make it easier for people)
 * Reads and parses packets asynchronously by running a parallel thread (see readPackets class)
 */

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;
import java.util.logging.Logger;
import com.fazecast.jSerialComm.SerialPort;

public class SerialCommunicator {
	private ArrayList<SerialPort> commList;
	protected SerialPort currPort;
	private InputStream in;
	private OutputStream out;
	private static final Logger log = Logger.getLogger(SerialCommunicator.class.getName());
	private boolean connected;
	private GUIController contrl;
	protected dataManager datM;
	protected String portName;
	private static Thread readThread;
	private readPackets reader;
	private Timer timer;
	protected long packsIn, packetTime;
	protected float packRate;
	private static boolean hold;
	
	//Connection constants
	private static final int BAUD_RATE = 115200;
	
    class ThreadCheck extends TimerTask  {
    	public boolean running;
    	@Override
    	public void run() {
    		try {
    			datM.printPackTime();
    		} catch(Exception e) {
    			log.severe("Pack time error!");
    			e.printStackTrace();
    		}
    		try {
        		resuscitate();
    		} catch(Exception e) {
    			log.severe("Reader thread error!");
    			e.printStackTrace();
    		}
    		contrl.updateWarnStyle();
    	}
    }
    
	//Constructor
	public SerialCommunicator(GUIController cont, dataManager _datM) {
		hold = false;
		datM = _datM;
		contrl = cont;
		datM.passComm(this);
		connected = false;
		resuscitate();
		packsIn = -1;
		updatePackTime();
	}
	public void killThread()  {
		timer.cancel();
		boolean kill = false;
		while(!kill) {
			readThread.interrupt();
			kill = !readThread.isAlive();
		}
	}
	public void resuscitate() {
		if(hold)
			return;
		hold = true;
		boolean read = readThread == null, time = timer == null;
		if(time) {
			timer = new Timer();
			timer.schedule(new ThreadCheck(), 0, 100);
			log.severe("Timer was started!");
		}
		
		if(read || !readThread.isAlive()) {
			reader = new readPackets(this);
			readThread = reader.start();
			if(read)
				log.severe("Thread was just resuscitated!");
		}
		hold = false;
	}
    protected void updatePackTime() {
    	packRate = (float)1000/getPackTime();
    	packetTime = System.currentTimeMillis(); 
    	packsIn++;
	}
    protected long getPackTime() {return System.currentTimeMillis() - packetTime; }
	public boolean threadAlive() {return readThread.isAlive(); }
	public SerialPort getSerialPort() {return currPort; }
	protected InputStream getInputStream() {return in; }
	public boolean getPortStatus() {
		if(currPort != null)
			return currPort.isOpen(); 
		return false;
	}
	public void updateCommList() {commList = new ArrayList<SerialPort>(Arrays.asList(SerialPort.getCommPorts())); }
	public ArrayList<String> getCommList() {
		updateCommList();
		ArrayList<String> newList = new ArrayList<>();
		for(SerialPort pt : commList)
			newList.add(pt.getDescriptivePortName());
		return newList;
	}
	public int getPortId(String portName) {
		log.finer(portName + "'s index is at " + getCommList().indexOf(portName));
		return getCommList().indexOf(portName);
	}
	public boolean openConnection(int portId) {
		if(portId < 0 || commList.size() - 1 < portId) {
			log.fine("Port index out of range.");
			return false;
		}
		portName = commList.get(portId).getDescriptivePortName();
		currPort = SerialPort.getCommPort(commList.get(portId).getSystemPortName());
		if(!currPort.openPort()) {
			log.severe("Failed to open connection");
			return false;
		} 
		log.info("Connection established.");
		currPort.setBaudRate(BAUD_RATE);
		out = currPort.getOutputStream();
		in = currPort.getInputStream();
		contrl.connButtSt(true);
		flushInput(Integer.MAX_VALUE);
		connected = true;
		
		return true;
	}
	public boolean closeConnection() {
		connected = false;
		if(currPort.closePort()) {
			log.info("Connection closed");
			in = null;
			out = null;
			return true;
		}
		log.severe("Failed to close connection!");
		connected = true;
		return false;
	}
	public void flushInput() {
		flushInput(200);
	}
	public void flushInput(int lim) {
		int limit = 0;
		try {
			while(in.available() > 0 && ++limit < lim)
				in.read();
		} catch (IOException e) {
			log.severe("Failed to read input buffer");
			log.severe(e.toString());
		}
		log.finer("Successfully flushed input buffer");
	}
	public boolean getState() {return connected; }
	public void getConnInfo() {
		if(!connected) {
			log.info("Not connected.");
			return;
		}
		String info = "Connected to: " + portName;
		info += ", running at "  + currPort.getBaudRate() + " baud";
		info += ", with " + currPort.getNumDataBits() + " data bits";
		info += ", with a timeout of " + currPort.getReadTimeout() + "ms for read, and ";
		info += currPort.getWriteTimeout() + "ms for write";
		log.info(info);
	}
	public void sendByte(byte b) {
		if(!connected) {
			log.severe("Failed to send " + b + ", no connection.");
			return;
		}
		byte[] bA = {b};
		currPort.writeBytes(bA, 1);
		log.finest(b + " sent");
	}
	public void writeData(byte[] b) {
		if(!connected) {
			log.severe("Failed to send " + b + ", no connection.");
			return;
		}
		currPort.writeBytes(b, b.length); 
		log.finest(b + " sent");
	}
}
