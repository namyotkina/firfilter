package com.tolmachoff.navreader;

import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.control.ToggleButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.input.DragEvent;
import javafx.scene.input.TransferMode;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;

import java.io.*;
import java.net.*;


public class Main extends Application {

    private Thread thread;




    private static class ReadCycle implements Runnable {
        private String filename;



        public ReadCycle(String filename) {
            this.filename = filename;

        }

        @Override
        public void run() {
            System.out.println("[i] Thread start");
            FileInputStream fileInputStream = null;
            byte[] bytes = new byte[808];
            try {
                fileInputStream = new FileInputStream(filename);
            }
            catch (FileNotFoundException e) {
                System.out.println("[E] Error opening file!");
                return;
            }

            DatagramSocket datagramSocket = null;
            try {
                datagramSocket = new DatagramSocket();
            }
            catch (SocketException e) {
                System.out.println("[E] Something bad with socket creation...");
                return;
            }

            while (true) {
                try {
                    if (fileInputStream.available() > 0) {
                        /*
                        fileInputStream.read(bytes);
                        datagramSocket.send(new DatagramPacket(bytes, 808, InetAddress.getLocalHost(), 4661));
                        */
                        fileInputStream.read(bytes, 0, 4);
                        String header = new String(bytes, 0, 4);
                        if (header.equals("_TRG")) {
                            fileInputStream.read(bytes, 4, 8);
                            datagramSocket.send(new DatagramPacket(bytes, 12, InetAddress.getLocalHost(), 4661));
                            System.out.println("[i] Trigger");
                        }
                        else if (header.startsWith("_CH")) {
                            fileInputStream.read(bytes, 4, 804);
                            datagramSocket.send(new DatagramPacket(bytes, 808, InetAddress.getLocalHost(), 4661));
                        }
                        else {
                            continue;
                        }
                    }
                    else {
                        System.out.println("[i] All done!");
                        Thread.currentThread().interrupt();
                    }
                }
                catch (IOException e) {
                    System.out.println("[E] Error reading file or UDP sending!");
                    return;
                }

                try {
                    Thread.sleep(1);
                }
                catch (InterruptedException e) {
                    System.out.println("[i] Thread interrupted");
                    break;
                }
            }
        }
    }


    @Override
    public void start(Stage primaryStage) throws Exception {
        Label label1 = new Label("Имя файла:");
        label1.setMinWidth(200);
        TextField text1 = new TextField("<перетащи сюда>");
        text1.setOnDragOver(new EventHandler<DragEvent>() {
            @Override
            public void handle(DragEvent event) {
                if (event.getGestureSource() != text1) {
                    event.acceptTransferModes(TransferMode.COPY);
                }
                event.consume();
            }
        });
        text1.setOnDragDropped(new EventHandler<DragEvent>() {
            @Override
            public void handle(DragEvent event) {
                text1.setText(event.getDragboard().getFiles().get(0).toString());
                event.setDropCompleted(true);
                event.consume();
            }
        });
        ToggleButton button1 = new ToggleButton("Воспроизведение");
        ToggleGroup group1 = new ToggleGroup();
        group1.getToggles().addAll(button1);
        group1.selectedToggleProperty().addListener(event -> {
            if (group1.getSelectedToggle() == button1) {
                thread = new Thread(new ReadCycle(text1.getText()));
                thread.start();
            }
            else {
                thread.interrupt();
                try {
                    thread.join();
                }
                catch (InterruptedException e) {
                    System.out.println(e);
                }
            }
        });
        GridPane root = new GridPane();
        root.setPadding(new Insets(10));
        root.setHgap(5);
        root.setVgap(5);
        root.add(label1, 0, 0);
        root.add(text1, 1, 0);
        root.add(button1, 1, 1);
        Scene scene = new Scene(root, 400, 80);
        primaryStage.setScene(scene);
        primaryStage.setResizable(false);
        primaryStage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
