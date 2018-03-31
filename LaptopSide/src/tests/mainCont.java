package tests;

import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;

public class mainCont {
	realTimePlot plot;
	int ind;
	
	@FXML
	Canvas plotCan;
	
	@FXML
	public void plotAxis() {
		plot.plotAxis();
	}
	@FXML
	public void plotTest() {
		int _ind = 0;
		while(++_ind < 100)
			plot.newPoint(Math.random());
			//plot.newPoint(3*Math.cos(++ind * Math.PI / 50));
		plot.replot();
	}
	@FXML
	public void testMore() {
		plot.newPoint(Math.random());
		//plot.newPoint(3*Math.cos(++ind * Math.PI / 50));
		plot.replot();
	}
	@FXML
	public void clear() {plot.clear(); }
	@FXML
	public void reset() {plot.reset(); }
	@FXML
	public void initialize() {
		plot = new realTimePlot(plotCan);
		plot.initialize();
	}
}
