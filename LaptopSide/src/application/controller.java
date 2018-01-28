package application;

import java.io.InputStream;
import java.util.ArrayList;

import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.MenuButton;
import javafx.scene.control.MenuItem;
import javafx.scene.control.TextArea;
import javafx.stage.Stage;

public class controller {
	InputStream in;
	SerialCommunicator comm;
	long bytesIn;
	
	//FXML declarations
	@FXML
	Button closeButt;
	@FXML
	MenuButton connMenu;
	@FXML 
	Button conUpButt;
	@FXML
	TextArea infoPane;
	@FXML
	Button discButt;
	@FXML
	Button connInfo;
	@FXML
	Label bytes;
	
	
	public controller() {
		comm = new SerialCommunicator(this);
		bytesIn = 0;
	}
	public void initialize() {
		connButtSt(false);
	}
	public void connButtSt(boolean st) {
		discButt.setDisable(!st);
		connInfo.setDisable(!st);
	}
	public void close() {
		if(comm.getState() && !comm.closeConnection()) {
			System.out.println("Failed to close connection");
			return;
		}
		System.out.println("Interface closed.");
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
					System.out.println(((MenuItem)e.getSource()).getText() + " prompted for connection.");
					comm.openConnection(comm.getPortId(((MenuItem)e.getSource()).getText()));
					//datM.initReadList();
				}
			});
		}
		System.out.println("Updated list of serial devices.");
	}
	public void disconnect() {
		if(!comm.getState())
			return;
		if(comm.closeConnection())
			connButtSt(false);
	}
	public void upBytes() {
		bytes.setText(Long.toString(bytesIn));
	}
	public void getConInfo()  {comm.getConnInfo(); }
}
