import java.awt.*;
import java.awt.event.*;
import java.io.*;
import javax.swing.*;
import javax.media.*;

public class JMFTest
    extends JFrame {
    private Player player;
    private File file;

    public JMFTest() {
        super("JMF-測試程式(by Sungo)");

        JButton openFile = new JButton("請選取要播放的音效檔 (*.Wav、*.MP3、*.MIDI..)");
        openFile.setFont(new java.awt.Font("細明體", 0, 15));
        openFile.setForeground(new Color(255, 255, 255));
        openFile.setBackground(new Color(204, 102, 255));
        openFile.addActionListener(
            new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                openFile();
                createPlayer();
            }
        }
        );
        getContentPane().add(openFile, BorderLayout.SOUTH);

        setSize(365, 84);
        show();
        try {
            jbInit();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void openFile() {
        JFileChooser fileChooser = new JFileChooser();

        fileChooser.setFileSelectionMode(
            JFileChooser.FILES_ONLY);
        int result = fileChooser.showOpenDialog(this);
        if (result == JFileChooser.CANCEL_OPTION) {
            file = null;
        }
        else {
            file = fileChooser.getSelectedFile();
        }
    }

    private void createPlayer() {
        if (file == null) {
            return;
        }
        removePreviousPlayer();
        try {
            player = Manager.createPlayer(file.toURL());
            player.addControllerListener(new EventHandler());
            player.start();
        }
        catch (Exception e) {
            JOptionPane.showMessageDialog(this,
                                          "檔案類型錯誤！或是找不到該檔案位置！",
                                          "錯誤訊息提示",
                                          JOptionPane.ERROR_MESSAGE);
        }
    }

    private void removePreviousPlayer() {
        if (player == null) {
            return;
        }
        player.close();
        Component visual = player.getVisualComponent();
        Component control = player.getControlPanelComponent();
        Container c = getContentPane();
        if (visual != null) {
            c.remove(visual);
        }
        if (control != null) {
            c.remove(control);
        }
    }

    public static void main(String args[]) {
        JMFTest app = new JMFTest();
        app.addWindowListener(
            new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }
        }
        );
    }

    private class EventHandler
        implements ControllerListener {
        public void controllerUpdate(ControllerEvent e) {
            if (e instanceof RealizeCompleteEvent) {
                Container c = getContentPane();
                Component visualComponent =
                    player.getVisualComponent();
                if (visualComponent != null) {
                    c.add(visualComponent, BorderLayout.CENTER);

                }
                Component controlsComponent =
                    player.getControlPanelComponent();

                if (controlsComponent != null) {
                    c.add(controlsComponent, BorderLayout.NORTH);

                }
                c.doLayout();
            }
        }
    }

    private void jbInit() throws Exception {
        this.setDefaultCloseOperation(HIDE_ON_CLOSE);
        this.getContentPane().setBackground(new Color(0, 102, 255));
        this.setResizable(false);
    }
}
