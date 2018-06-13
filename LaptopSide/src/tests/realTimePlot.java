package tests;

import java.text.DecimalFormat;
import java.util.ArrayList;
import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.text.Text;

public class realTimePlot {
	Canvas plotCan;
	private boolean canInit = false;
	private static final DecimalFormat df = new DecimalFormat("#.##");
	double canWidth, canHeight, xCanOffset, yCanOffset, plotHeight, plotWidth;
	double maxY, minY, xScale, yScale;
	float XMargin, YMargin;
	int minTime, maxTime, tNot, cnt;
	String xAxisLabel, yAxisLabel;
	GraphicsContext gc;
	point plotOrigin, xAxisLimit, yAxisLimit;
	ArrayList<point> data;
	Text[] axisTicks = new Text[4];
	Text xLabel, yLabel;
	
	static final int tickWidth = 8;
	static final double[] marks = {.25, .5, .75, 1};
	static final float XAxisOffset = (float) 0.2, YAxisOffset = (float) 0.2;
	static final float XMarginRatio = (float) 0.05, YMarginRatio = (float) 0.05;
	
	public realTimePlot(Canvas _plotCan) {
		plotCan = _plotCan;
	}
	
	//Point class definition
	public class point {
		public double x, y;
		public point(double _x, double _y) {
			x = _x;
			y = _y;
		}
		public point(double _y) {
			x = tNot;
			y = _y;
		}
		@Override
		public String toString() {return "(" + x + ", " + y + ")"; }
		public point timeShift() {this.x--; return this;}
	}
	//Initialization methods
	public void initialize() {
		axisLabel("Time", "Force");
		data = new ArrayList<point>();
		maxY = minY = 0;
		
		gc = plotCan.getGraphicsContext2D();
		canWidth = plotCan.getWidth();
		canHeight = plotCan.getHeight();
		XMargin = (float) canWidth * XMarginRatio;
		YMargin = (float) canHeight * YMarginRatio;
		plotAxis();
		plotScale();
	}
	@FXML
	void plotAxis() {
		//Declare variables
		plotOrigin = new point(XMargin, YMargin);
		xAxisLimit = new point(canWidth - XMargin, YMargin);
		yAxisLimit = new point(XMargin, canHeight - YMargin);
		xCanOffset = plotCan.getLayoutX(); 
		yCanOffset = plotCan.getLayoutY();
		tNot = (int)(xAxisLimit.x - plotOrigin.x - 4);
		double xLabelCoord = getAverage(plotOrigin.x, xAxisLimit.x), yLabelCoord = getAverage(plotOrigin.y, yAxisLimit.y);
				
		//Draw axis
		gc.strokeLine(plotOrigin.x, canHeight - plotOrigin.y, xAxisLimit.x, canHeight - xAxisLimit.y);
		gc.strokeLine(plotOrigin.x, canHeight - plotOrigin.y, yAxisLimit.x, canHeight - yAxisLimit.y);
		
		//Axis labels
		xLabel = new Text(xLabelCoord + xCanOffset, canHeight - (plotOrigin.y - (YAxisOffset * canHeight * .25)) + yCanOffset, xAxisLabel);
		xLabel.setX(xLabelCoord + xCanOffset - xLabel.getLayoutBounds().getWidth() / 2);
		yLabel = new Text(xCanOffset + XMargin - 25, yLabelCoord + yCanOffset, yAxisLabel);
		yLabel.setRotate(-90);
		plotTest.root.getChildren().addAll(xLabel, yLabel);
		canInit = true;
	}
	
	//Utility methods
	double getAverage(double a, double b) {return (a + b) / 2; }
	private boolean checkLimits(point newPoint) {return newPoint.y < minY || newPoint.y > maxY; }
	private void calcScale() {
		if(!canInit) plotAxis();
		for(point pt : data) {
			if(pt.y < minY) minY = pt.y;
			else if(pt.y > maxY) maxY = pt.y;
		}
		yScale = yAxisLimit.y*.95 / (maxY - minY); 
		updateTicks();
	}
	
	//Plot configuration
	public void xLabel(String label) {xAxisLabel = label; }
	public void yLabel(String label) {yAxisLabel = label; }
	public void axisLabel(String _xLabel, String _yLabel) {
		xLabel(_xLabel);
		yLabel(_yLabel);
	}
	
	//Direct plotting methods
	public void replot() {
		if(!canInit) plotAxis();
		clear(); 
		for(point pt : data) plotPoint(pt); 
	}
	private void plotPoint(point newPoint) {
		if(newPoint.x < XMargin + 1) return;
		point coord = new point(newPoint.x, canHeight - (YMargin + 2 + Math.abs(minY)*yScale + newPoint.y*yScale));
		//System.out.println(coord);
		gc.strokeRect(coord.x, coord.y, 1, 1);
	}
	
	public void plotScale() {
		boolean fakeMax = maxY == 0;
		maxY = fakeMax? 1 : maxY;
		point tmp;
		double maxWidth = 0, tmpWidth;
		for(int i=0;i<marks.length;i++) {
			tmp = new point(plotOrigin.x-tickWidth, canHeight - (YMargin + 2 + Math.abs(minY)*yScale + marks[i]*maxY*yScale) - 1);
			gc.strokeRect(tmp.x, tmp.y, tickWidth, 1);
			axisTicks[i] = new Text(plotOrigin.x-tickWidth+xCanOffset, tmp.y + yCanOffset + 5, df.format(marks[i]*maxY));
			tmpWidth = axisTicks[i].getLayoutBounds().getWidth();
			axisTicks[i].setX(plotOrigin.x-tickWidth+xCanOffset - tmpWidth*1.2);
			maxWidth = (maxWidth < tmpWidth)? tmpWidth : maxWidth;
		}
		yLabel.setX(xCanOffset + XMargin - 25 - maxWidth*1.5);
		plotTest.root.getChildren().addAll(axisTicks);
		maxY = fakeMax? 0 : maxY;
	}
	private void updateTicks() {
		double maxWidth = 0, tmpWidth;
		for(int i=0;i<axisTicks.length;i++) {
			axisTicks[i].setText(df.format(marks[i]*maxY));
			tmpWidth = axisTicks[i].getLayoutBounds().getWidth();
			axisTicks[i].setX(plotOrigin.x-tickWidth+xCanOffset - tmpWidth*1.2);
			maxWidth = (maxWidth < tmpWidth)? tmpWidth : maxWidth;
		}
	}
	//Data management methods
	public void clear() {
		point size = new point(xAxisLimit.x - plotOrigin.x, yAxisLimit.y - plotOrigin.y + 5);
		point base = new point(plotOrigin.x + 1, plotOrigin.y + 1 + size.y);
		int tickBase = 15;
		
		gc.clearRect(base.x, canHeight - base.y, size.x, size.y); 
		System.out.println(yAxisLimit.y - plotOrigin.y);
		gc.strokeRect(base.x - tickBase - 2, canHeight - base.y + 6, tickBase, yAxisLimit.y - plotOrigin.y);
	}
	@FXML
	public void reset() {clear(); data.clear(); }
	public void newPoint(double newVal) {newPoint(new point(newVal)); }
	public void newPoint(point newPoint) {
		if(!canInit) plotAxis();
		for(int i=0;i<data.size();i++) data.set(i, data.get(i).timeShift());
		data.add(newPoint);
		if(checkLimits(newPoint)) calcScale();
	}
}
