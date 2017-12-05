import java.io.Serializable;

/**
 * Holds the structure of a post message between the server and the client
 * Holds the message that should be added to a post and methods to add and extract that message
 */

public class PostMessage implements Serializable {
    private String msg;

    /**
     * Method for adding wanted string to the message
     * @param msg the text that should be added to the message and later to the new post
     */
    
    public PostMessage(String msg) {
        this.msg = msg;
    }

    /**
     * Method for extracting the string from the message
     * @return the string that should be added to a post
     */
    
    public String getMsg() {
        return msg;
    }
}
