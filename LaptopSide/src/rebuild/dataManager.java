package rebuild;

import java.nio.ByteBuffer;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.logging.Logger;
import javafx.application.Platform;

public class dataManager {
	private float distToOrg, distTrav;
	private float altAlt, altGPS, speed, HDOP, sValid, battState;
	private int satelites, btsAv;
	private String fixType;
	private GUIController cont;
	private SerialCommunicator comm;
	private static final Logger log = Logger.getLogger(dataManager.class.getName());
	private static final DecimalFormat df = new DecimalFormat("#.##");
	private static final DecimalFormat df4 = new DecimalFormat("#.####");
	
	//Transmission constants (all private by default, not written for clarity)
	static final char PACKET_START = '*', PACKET_END = 'e';
	
	//Class Constructor
	public dataManager(GUIController _cont) {
		cont = _cont;
		distToOrg = distTrav = battState = HDOP = altAlt = altGPS = speed = sValid = satelites = btsAv = 0;
		update();
	}	
	//Formats number to 2 decimal places
	public String getFormatted(double inp) {return df.format(inp); }
	
	//Formats number to 4 decimal places
	public String getFormatted4(double inp) {return df4.format(inp); }
	
	//Passes communicator class
	public void passComm(SerialCommunicator _comm) {comm = _comm; }
	
	//Checks status of connections for warnings
	public void statusChecks(double packDelta) {
		boolean badConn = false;
				//Serial Connection
		if(cont.getSW())
			cont.setSW(false);
		if(badConn && !comm.getPortStatus())
			cont.setSW(true);
	}
	public void printPackTime() {
		double packDelta = (double)comm.getPackTime() / 1000;
		Platform.runLater(new Runnable() {
			@Override
			public void run() {cont.packTime.setText("Pack Time: " + df.format(packDelta) + 's'); }
		});
		statusChecks(packDelta);
	}
	private void update() {
		cont.updateWarnText();
		Platform.runLater(new Runnable()  {
			@Override
			public void run() {
				cont.bytesAvail.setText("Bytes availible: " + btsAv);
				cont.packetsR.setText("Packets received: " + comm.packsIn);
				cont.packRateLabel.setText("Pack rate: " + df.format(comm.packRate) + " packs/s");
				if(comm != null && comm.getState())
					cont.connState.setText("Connection state: Connected to " + comm.portName);
				else
					cont.connState.setText("Connection state: Disconnected.");
			}
		});
	}
	public void newPacket(byte[] pack) {newPacket(pack, -1); }
	//Reorders incoming bytes from the Xbee (i.e. 4321 -> 1234)
	private byte[] flipBytes(byte[] set) {
		int len = set.length;
		byte[] out = new byte[len];
		for(int i=0;i<len;i++)
			out[i] = set[len-i-1];
		return out;
	}
	//Parses new incoming data packet
	public void newPacket(byte[] pack, int _btsAv) {
		float lat = 0, lng = 0;
		for(int i=2;i<31;i+=4) {
			byte[] tmp = new byte[4];
			System.arraycopy(pack, i, tmp, 0, 4);
			tmp = flipBytes(tmp);
			if(i == 2)
				altAlt = ByteBuffer.wrap(tmp).getFloat();
			else if(i == 6)
				speed = ByteBuffer.wrap(tmp).getFloat();
			else if(i == 10)
				lng = ByteBuffer.wrap(tmp).getFloat();
			else if(i == 14)
				lat = ByteBuffer.wrap(tmp).getFloat();
			else if(i == 18)
				HDOP = ByteBuffer.wrap(tmp).getFloat();
			else if(i == 22)
				sValid = ByteBuffer.wrap(tmp).getFloat();
			else if(i == 26)
				altGPS = ByteBuffer.wrap(tmp).getFloat();
			else if(i == 30)
				battState = ByteBuffer.wrap(tmp).getFloat();
		}
		btsAv = _btsAv;
		update();
	}
}
