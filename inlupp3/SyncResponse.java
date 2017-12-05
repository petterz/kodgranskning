import java.io.Serializable;
import java.util.Set;
import java.util.List;

/** 
 * Class holding the structure of a message between the server and client of type SyncResponse
 * Holds methods for adding and extracting information from the message
 */

public class SyncResponse implements Serializable {
    private Set<Account> users;
    private List<Post> posts;

    /**
     * Constructs a new sync response with the given parameters that is included in the message
     * @param users set of all the users in that should be added to the response normally the amount of users in the server at the moment
     * @param posts list of all the posts that should be included in the response normally all the posts thats in the servers feed 
     */
    
    public SyncResponse(Set<Account> users, List<Post> posts) {
        this.users = users;
        this.posts = posts;
    }

    /**
     * Method for extracting the list of posts from the message
     * @return the list of posts that has been added to the message
     */
    
    public List<Post> getPosts() {
        return this.posts;
    }

    /**
     * Method for extracting the set of account from the message
     * @return the set of accounts thats included in the message
     */
    
    public Set<Account> getUsers() {
        return this.users;
    }
}
