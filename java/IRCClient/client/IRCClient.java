package client;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.StringTokenizer;

public class IRCClient extends Thread implements Runnable {
	private Socket socket;

	private DataInputStream input;

	private DataOutputStream out;

	private String host;

	private String channel;

	@SuppressWarnings("deprecation")
	@Override
	public void run() {
		while (!isInterrupted()) {
			String line;
			try {
				line = input.readLine();
				if (line.length() > 0) {
					System.out.println(line);
					processCMD(line);					
					if (ircCLientListener != null)
						ircCLientListener.onMessage(line);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	private void processCMD(String cmd) throws IOException {
		StringTokenizer st = new StringTokenizer(cmd);
		String token1 = st.nextToken();

		if (token1.equals("PING")) {
			sendCMD("PONG " + st.nextToken());
		}
	}

	private void sendCMD(String cmd) throws IOException {
		System.out.println(">" + cmd);
		out.writeBytes(cmd + "\n");
	}

	public IRCClient() {
		socket = new Socket();
	}

	public interface IRCCLientListener {
		public void onMessage(String msg);
	}

	private IRCCLientListener ircCLientListener = null;

	public void setIRCCLientListener(IRCCLientListener ircCLientListener) {
		this.ircCLientListener = ircCLientListener;
	}

	public void connect(String host, int port) throws IOException {
		this.host = host;

		socket.connect(new InetSocketAddress(host, port));

		out = new DataOutputStream(socket.getOutputStream());
		input = new DataInputStream(socket.getInputStream());

		start();
	}

	public void login(String user, String nick) throws IOException {
		sendCMD("NICK " + nick);
		sendCMD("USER " + user + " \"\"" + " \"" + host + "\" :" + nick);
	}

	public void join(String channel) throws IOException {
		this.channel = channel;
		sendCMD("JOIN " + channel);
	}

	public void msg(String target, String msg) throws IOException {
		sendCMD("PRIVMSG " + target + " :" + msg);
	}

	public void quit(String msg) throws IOException, InterruptedException {
		if (!isInterrupted())
			interrupt();

		// wait();

		if (socket.isConnected()) {
			sendCMD("QUIT :" + msg);

			socket.close();
		}
	}

	public void runCMD(String cmd) {
		StringTokenizer st = new StringTokenizer(cmd);
		String token1 = st.nextToken();

		if (token1.charAt(0) == '/') {
			if (token1.toLowerCase().equals("/server")) {
				try {
					connect(st.nextToken(), 6667);
					login(st.nextToken(), st.nextToken());
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			} else if (token1.toLowerCase().equals("/join")) {
				try {
					join(st.nextToken());
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			} else if (token1.toLowerCase().equals("/quit")) {
				try {
					String smsg = "";
					while (st.hasMoreTokens()) {
						if (smsg.length() > 0)
							smsg += " ";
						smsg += st.nextToken();
					}

					quit(smsg);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			} else if (token1.toLowerCase().equals("/priv")) {
				try {
					String target = st.nextToken();
					String smsg = st.nextToken();

					while (st.hasMoreTokens()) {
						smsg += " " + st.nextToken();
					}

					msg(target, smsg);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		} else {
			try {
				msg(channel, cmd);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}
