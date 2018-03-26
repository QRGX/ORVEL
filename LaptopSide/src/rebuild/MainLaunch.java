package rebuild;
	
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.scene.layout.GridPane;


public class MainLaunch extends Application {
	protected GridPane root;
	@Override
	public void start(Stage primaryStage) {
		try {	
			root = (GridPane)FXMLLoader.load(MainLaunch.class.getResource("MainScreen.fxml"));
			Scene scene = new Scene(root);
			//scene.getStylesheets().add("rebuild/application.css");
			primaryStage.setScene(scene);
			primaryStage.setMaximized(true);
			primaryStage.centerOnScreen();
			primaryStage.setTitle("RGX ORVEL");
			primaryStage.setResizable(false);
			primaryStage.show();
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) {
		launch(args);
		
	}
}
