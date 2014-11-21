import javax.media.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
public class MediaPlayerFrame extends JFrame {
    private static final String FRAME_TITLE = "簡易媒體播放GUI程式";
    private static final String CONTROL_PANEL_TITLE = "Control Panel";
    private static final int LOC_X = 100;
    private static final int LOC_Y = 100;
    private static final int HEIGHT = 500;
    private static final int WIDTH = 500;
    private Player player = null;
    private JTabbedPane tabPane = null;
    public MediaPlayerFrame() {
        super(FRAME_TITLE);
        setLocation(LOC_X, LOC_Y);
        setSize(WIDTH, HEIGHT);
        tabPane = new JTabbedPane();
        getContentPane().add(tabPane);
        addWindowListener(new WindowAdapter() {
                              public void windowClosing(WindowEvent e) {
                                  closeCurrentPlayer();  
                                  System.exit(0);
                              }
                          });
    }
    private JPanel createMainPanel() {
        JPanel mainPanel = new JPanel();
        GridBagLayout gbl = new GridBagLayout();
        GridBagConstraints gbc = new GridBagConstraints();
        mainPanel.setLayout(gbl);
        boolean visualComponentExists = false;
        if (player.getVisualComponent() != null) {
            visualComponentExists = true;
            gbc.gridx = 0;
            gbc.gridy = 0;
            gbc.weightx = 1;
            gbc.weighty = 1;
            gbc.fill = GridBagConstraints.BOTH;
            mainPanel.add(player.getVisualComponent(), gbc);
        }
        if ((player.getGainControl() != null) &&
            (player.getGainControl().getControlComponent() != null)) {
            gbc.gridx = 1;
            gbc.gridy = 0;
            gbc.weightx = 0;
            gbc.weighty = 1;
            gbc.gridheight = 2;
            gbc.fill = GridBagConstraints.VERTICAL;
            mainPanel.add(player.getGainControl().getControlComponent(), gbc);
        }
        if (player.getControlPanelComponent() != null) {
            gbc.gridx = 0;
            gbc.gridy = 1;
            gbc.weightx = 1;
            gbc.gridheight = 1;
            if (visualComponentExists) {
                gbc.fill = GridBagConstraints.HORIZONTAL;
                gbc.weighty = 0;
            } else {
                gbc.fill = GridBagConstraints.BOTH;
                gbc.weighty = 1;
            }
            mainPanel.add(player.getControlPanelComponent(), gbc);
        }
        return mainPanel;
    }
// 設定media locator
    public void setMediaLocator(MediaLocator locator) throws IOException, 
        NoPlayerException, CannotRealizeException {
        setPlayer(Manager.createRealizedPlayer(locator));
    }
    public void setPlayer(Player newPlayer) {
        closeCurrentPlayer();
        player = newPlayer;
        tabPane.removeAll();
        if (player == null) return;
        tabPane.add(CONTROL_PANEL_TITLE, createMainPanel());
        Control[] controls = player.getControls();
        for (int i = 0; i < controls.length; i++) {
            if (controls[i].getControlComponent() != null) {
                tabPane.add(controls[i].getControlComponent());
            }
        }
    }
    private void closeCurrentPlayer() {
        if (player != null) {
            player.stop();
            player.close();
        }
    }
    public static void printUsage() {
        System.out.println("使用方式 : java MediaPlayerFrame mediaLocator");
    }
    public static void main(String[] args) {
        try {
            if (args.length == 1) {
                MediaPlayerFrame mpf = new MediaPlayerFrame(); 
                mpf.setMediaLocator(new MediaLocator(new File(args[0]).toURL()));
                //mpf.setMediaLocator(new MediaLocator(args[0]));
	mpf.show();
            } else {
                printUsage();
            }
        } catch (Throwable t) {
            t.printStackTrace();
        }
    }
}

