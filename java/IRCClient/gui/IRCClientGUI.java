package gui;

import java.awt.BorderLayout;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;

import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import javax.swing.WindowConstants;
import javax.swing.SwingUtilities;

import client.IRCClient;
import client.IRCClient.IRCCLientListener;

/**
 * This code was edited or generated using CloudGarden's Jigloo SWT/Swing GUI
 * Builder, which is free for non-commercial use. If Jigloo is being used
 * commercially (ie, by a corporation, company or business for any purpose
 * whatever) then you should purchase a license for each developer using Jigloo.
 * Please visit www.cloudgarden.com for details. Use of Jigloo implies
 * acceptance of these licensing terms. A COMMERCIAL LICENSE HAS NOT BEEN
 * PURCHASED FOR THIS MACHINE, SO JIGLOO OR THIS CODE CANNOT BE USED LEGALLY FOR
 * ANY CORPORATE OR COMMERCIAL PURPOSE.
 */
public class IRCClientGUI extends javax.swing.JFrame implements
		IRCCLientListener {
	private static final long serialVersionUID = 1L;

	private JTextField jTextField1;

	private JScrollPane jScrollPane1;

	private JTextArea jTextArea1;

	private IRCClient ircClient;

	/**
	 * Auto-generated main method to display this JFrame
	 */
	public static void main(String[] args) {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				IRCClientGUI inst = new IRCClientGUI();
				inst.setLocationRelativeTo(null);
				inst.setVisible(true);
			}
		});
	}

	public IRCClientGUI() {
		super();
		initGUI();
		ircClient = new IRCClient();
		ircClient.setIRCCLientListener(this);
	}

	private void initGUI() {
		try {
			setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
			this.setTitle("IRC Client");
			this.addWindowListener(new WindowAdapter() {
				public void windowClosed(WindowEvent evt) {
					thisWindowClosed(evt);
				}
			});
			{
				jTextField1 = new JTextField();
				getContentPane().add(jTextField1, BorderLayout.SOUTH);
				jTextField1.setText("/server irc.rizon.net user nick");
				jTextField1.addKeyListener(new KeyAdapter() {
					public void keyReleased(KeyEvent evt) {
						jTextField1KeyReleased(evt);
					}
				});
			}
			{
				jScrollPane1 = new JScrollPane();
				getContentPane().add(jScrollPane1, BorderLayout.CENTER);
				{
					jTextArea1 = new JTextArea();
					jScrollPane1.setViewportView(jTextArea1);
				}
			}
			pack();
			this.setSize(665, 460);
		} catch (Exception e) {
			// add your error handling code here
			e.printStackTrace();
		}
	}

	private void jTextField1KeyReleased(KeyEvent evt) {
		if (evt.getKeyChar() == 10)
			ircClient.runCMD(jTextField1.getText());
	}

	public void onMessage(String msg) {
		jTextArea1.append(msg + "\n");
	}

	private void thisWindowClosed(WindowEvent evt) {
		try {
			ircClient.quit("");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
