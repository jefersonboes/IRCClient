package client;

public class CMDParser {
	private String cmd;

	private int offset;

	public CMDParser(String cmd) {
		this.cmd = cmd;
		offset = 0;
	}

	public String getNextToken() {
		String token = "";

		while (offset < cmd.length()) {
			if (cmd.charAt(offset) == ' ')
				return token;
			else
				token += cmd.charAt(offset);

			offset++;
		}

		return token;
	}

	public String getString() {
		String string = "";

		while (offset < cmd.length()) {
			string += cmd.charAt(offset);
			offset++;
		}

		return string;
	}

	public boolean isCMD() {
		if (cmd.length() > 0)
			if (cmd.charAt(0) == '/')
				return true;

		return false;
	}

	public void skipCMDToken() {
		if (isCMD())
			offset = 1;
	}

	public String getCMD() {
		String scmd = "";

		if (isCMD()) {
			skipCMDToken();

			scmd = getNextToken();
		}

		return scmd;
	}
}
