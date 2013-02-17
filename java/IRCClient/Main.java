import java.io.IOException;

import client.IRCClient;

public class Main {

	public static void main(String[] args) throws IOException {
		IRCClient ircClient = new IRCClient();

		ircClient.connect("irc.rizon.net", 6667);
		ircClient.login("Nitronix_Ex", "Nitronix_Ex");
		// ircClient.join("#Kaze-Hime");
		// ircClient.msg("#Kaze-Hime", "testing IRC java client");
	}
}
