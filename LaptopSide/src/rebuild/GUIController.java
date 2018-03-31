package rebuild;

import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.logging.FileHandler;
import java.util.logging.Handler;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;
import java.util.logging.StreamHandler;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.MenuButton;
import javafx.scene.control.MenuItem;
import javafx.scene.control.Tab;
import javafx.scene.control.TextArea;
import javafx.scene.image.Image;
import javafx.scene.input.KeyEvent;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.stage.Stage;
import org.json.*;

public class GUIController {
	//FXML declarations
	@FXML
	Pane MapPane;
	@FXML
	GridPane mainPane;
	@FXML
	MenuButton mapList, connMenu;
	@FXML
	TextArea infoPane;
	@FXML
	Button connInfo, discButt, tempButt, closeButt;

	//Status labels
	@FXML
	Canvas mapCan;
	@FXML
	Label vHDOP, packTime, speed, bytesAvail, satNum, heightL, pointsTaken, mapTop, mapBase;
	@FXML
	Label orginDist, travDist, batLevel, hdop, connState, packetsR, packRateLabel, fixTp;
	
	//Warnings
	@FXML
	Label xbeeState, GPSState, battState, serialState, mapState, onMapStatus;
	private boolean xbeeWarn, GPSWarn, battWarn, serialWarn, warnChange, mapWarn;
	
	public void setXW(boolean st) {xbeeWarn = st; warnC(); }
	public void setGW(boolean st) {GPSWarn = st; warnC(); }
	public void setBW(boolean st) {battWarn = st; warnC(); }
	public void setSW(boolean st) {serialWarn = st; warnC(); }
	public boolean getXW() {return xbeeWarn; }
	public boolean getGW() {return GPSWarn; }
	public boolean getBW() {return battWarn; }
	public boolean getSW() {return serialWarn; }
	private void warnC() {warnChange = true; }
	
	//Map variables
	float baseXOffset, baseYOffset;
	realTimePlot plot;
	//public Canvas mapCan;
	private GraphicsContext gc;
	
	//Communication
	SerialCommunicator comm;
	dataManager datM;
	OutputStream infOut;
	private static final Logger log = Logger.getLogger(GUIController.class.getName());
	FileHandler filehandle = null;
	
	@FXML
	public void initialize() {
		datM = new dataManager(this);	//Works with bytes from comm
		comm = new SerialCommunicator(this, datM);	//Raw communication with Xbee
		connButtSt(false);
		FileHandler filehandle = null;
		try {
			filehandle = new FileHandler("output.log");
		} catch (IOException e) {
			e.printStackTrace();
		}
		log.fine("Initializing.");
		xbeeWarn = GPSWarn = battWarn = serialWarn = mapWarn = true;
	}
	
	//Initializes the canvas
	public void initCanvas() {
		if(mapCan != null) return;
		mapCan = new Canvas(MapPane.getWidth(), MapPane.getHeight());
		MapPane.getChildren().add(mapCan);
		mapCan.setTranslateX(MapPane.getLayoutX());
		mapCan.setTranslateY(MapPane.getLayoutY());
		initPlot();
		log.info("Canvas initialized.");
	}
	private void initPlot() {
		plot = new realTimePlot(this, mapCan);
		plot.initialize();
	}
	@FXML
	public void testDraw() {
		if(mapCan == null) initCanvas();
		if(!plot.getCanStat()) plot.plotAxis();
		plot.newPoint(Math.random());
		plot.replot();
	}
	@FXML
	public void checkThread() {
		if(comm.threadAlive())
			log.info("Thread is alive and well.");
		else {
			log.info("Thread is dead!");
			comm.resuscitate();
		}
	}
	@FXML
	public void close() {
		comm.killThread();
		if(comm.getState() && !comm.closeConnection()) {
			log.severe("Failed to close connection");
			return;
		}
		log.info("Interface closed.");
		((Stage)(closeButt.getScene().getWindow())).close();
	}
	public void updateCons() {
		ArrayList<String> cons = comm.getCommList();
		connMenu.getItems().clear();
		for(String con : cons) {
			MenuItem newItem = new MenuItem(con);
			connMenu.getItems().add(newItem);
			newItem.setOnAction(new EventHandler<ActionEvent>() {
				@Override
				public void handle(ActionEvent e) {
					new Thread(new Runnable() {
						@Override
						public void run() {
							log.fine(((MenuItem)e.getSource()).getText() + "Prompted for connection.");
							comm.openConnection(comm.getPortId(((MenuItem)e.getSource()).getText()));
						}
					}).start();
				}
			});
		}
		log.fine("Updated list of serial devices.");
	}
	public void disconnect() {
		if(!comm.getState())
			return;
		if(comm.closeConnection())
			connButtSt(false);
	}
	public void connButtSt(boolean st) {
		discButt.setDisable(!st);
		connInfo.setDisable(!st);
	}
	//NOTE: adjust lat conditions to work anywhere when time - lazy rn...
	public String getStyleStr(boolean tp) {
		if(tp)
			return "-fx-font-weight: bold;";
		return "-fx-font-weight: normal;";
	}
	public String getStateStr(boolean tp) {
		if(!tp)
			return "Normal";
		return "WARNING";
	}
	public void updateWarnText() {
		Platform.runLater(new Runnable() {
			@Override
			public void run() {
				serialState.setText("Serial State: " + getStateStr(serialWarn));
				battState.setText("Battery State: " + getStateStr(battWarn));
			}
		});
	}
	public void updateWarnStyle() {
		if(!warnChange)
			return;
		warnChange = false;
		if(xbeeWarn)
			updateWarnText();
		Platform.runLater(new Runnable() {
			@Override
			public void run() {
				serialState.setStyle(getStyleStr(serialWarn));
				battState.setStyle(getStyleStr(battWarn));
			}
		});	
	}
	public void getConInfo()  {comm.getConnInfo(); }
}
