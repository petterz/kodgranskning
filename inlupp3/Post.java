import java.io.Serializable;

/**
 * Class for the structure of a post 
 * Hold information about the posts id, the poster and the content of the post
 * Aswell as methods for adding and extracting that information
 */

public class Post implements Serializable {
    private int globalPostId;
    private Account poster;
    private String content;

    /**
     * Constructs a post with the given parameters
     * @param globalPostId the id that should be set to the post
     * @param poster the account to be linked with the post
     * @param content the content that should be added to the post
     */

    public Post(int globalPostId, Account poster, String content) {
        this.globalPostId = globalPostId;
        this.poster       = poster;
        this.content      = content;
    }

    /**
     * Method for rendering a post
     * Adds the posters name to the content and returns a new string
     * @return a string with the content combinded with the posters name
     */
    
    public String render() {
        return "{" + this.poster.getName() + "} says:\n" + this.content + "\n";
    }

    /**
     * Method for linking an account to the post
     * @param newUser the account that should be linked with the post
     */
    
    public void setPoster(Account newUser) {
        this.poster=newUser;
    }

    /**
     * Method for takeing the poster out of the message
     * @return the poster account of the post
     */
    
    public Account getPoster() {
        return this.poster;
    }
}
