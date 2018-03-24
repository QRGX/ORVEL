package tests;

import tests.plotTest;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;

public class plotTest extends Application {
	protected static Pane root;

	@Override
	public void start(Stage primaryStage) {
		try {
			root = (Pane)FXMLLoader.load(plotTest.class.getResource("arrows.fxml"));
			Scene scene = new Scene(root);
			primaryStage.setScene(scene);
			primaryStage.centerOnScreen();
			primaryStage.setTitle("Plotting test");
			primaryStage.show();
		} catch(Exception e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		launch(args);
	}
}
