package application;

/** Notes on Class:
 * Based on the code from the old Java Swing version of the ground station
 * Uses a serial library that (should) work for all operating systems and doesn't have any weird dependencies (to make it easier for people)
 */

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Arrays;
import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;

public class SerialCommunicator {
	private ArrayList<SerialPort> commList;
	private SerialPort currPort;
	private InputStream in;
	private boolean connected;
	protected String portName;
	protected ArrayList<Byte> byteBuff;
	private controller contrl;
	
	
	//Connection constants
	private static final int BAUD_RATE = 115200;
	
	//Constructor
	public SerialCommunicator(controller cont) {
		updateCommList();
		connected = false;
		byteBuff = new ArrayList<Byte>();
		contrl = cont;
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
		return getCommList().indexOf(portName);
	}
	public boolean openConnection(int portId) {
		if(portId < 0 || commList.size() - 1 < portId) {
			System.out.println("Port index out of range.");
			return false;
		}
		portName = commList.get(portId).getDescriptivePortName();
		currPort = SerialPort.getCommPort(commList.get(portId).getSystemPortName());
		if(!currPort.openPort()) {
			System.out.println("Failed to open connection");
			return false;
		} 
		System.out.println("Connection established.");
		currPort.setBaudRate(BAUD_RATE);
		in = currPort.getInputStream();
		connected = true;
		contrl.connButtSt(true);
		flushInput();
		initReadList();
		
		return true;
	}
	public boolean closeConnection() {
		if(currPort.closePort()) {
			System.out.println("Connection closed");
			connected = false;
			in = null;
			return true;
		}
		System.out.println("Failed to close connection!");
		return false;
	}
	public void flushInput() {
		int limit = 0;
		try {
			while(in.available() > 0 && ++limit < 200)
				in.read();
		} catch (IOException e) {
			System.out.println("Failed to read input buffer");
			System.out.println(e.toString());
		}
	}
	public boolean getState() {return connected; }
	public void getConnInfo() {
		if(!connected) {
			System.out.println("Not connected.");
			return;
		}
		String info = "Connected to: " + portName;
		info += ", running at "  + currPort.getBaudRate() + " baud";
		info += ", with " + currPort.getNumDataBits() + " data bits";
		info += ", with a timeout of " + currPort.getReadTimeout() + "ms for read, and ";
		info += currPort.getWriteTimeout() + "ms for write";
		System.out.println(info);
	}
	public void initReadList() {
		currPort.addDataListener(new SerialPortDataListener() {
			@Override
			public int getListeningEvents() {
				return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
			}
			@Override
			public void serialEvent(SerialPortEvent e) {
				if(e.getEventType() != SerialPort.LISTENING_EVENT_DATA_AVAILABLE)
					return;
				byte[] tmpBuff = new byte[currPort.bytesAvailable()];
				contrl.bytesIn += currPort.readBytes(tmpBuff, tmpBuff.length);
				toBuffer(tmpBuff);
			}
		});
		System.out.println("Added listener.");
	}
	private synchronized void toBuffer(byte[] tmp) {
		for(byte bt : tmp)
			byteBuff.add(bt);
	}
}
