import java.util.List;
import java.util.LinkedList;

/**
 * Class that holds the structure of a feed
 * Has methods for adding new posts to the feed, how to render them to a printable string and a printing method
 * Holds all the posts in the feed in a list
 */

public class Feed {
private List<Post> posts = new LinkedList<Post>();

    /**
     * Method for adding a new post to the feed
     * @param post the post that should be added
     */
    
    public void addPost(Post post) {
        posts.add(0, post);
    }
    
    /**
     * Method for rendering all posts in the feed utilizes the render method
     * @param user the account that uses the method, is used in the render method
     * @return a string of all the posts in the feed that the user is not ignoring
     */
    
    public String renderAll(Account user) {
        return this.render(posts.size(),user);
    }

    /** 
     * Method for rendering the posts from a given point
     * @param n the number of posts that should  be rendered
     * @return a string of the n latest posts from the feed
     */
    
    public String renderLatest(int n) {
        return this.render(n);
    }

    /**
     * Method to render n posts and check if the user is ignoring with the poster
     * @param n the ammount of posts to be rendered
     * @param user the account that wants the feed reference for ignored users
     * @return a string with n posts that the user is not ignoring     
     */
    
    private String render(int n, Account user) {
        String result = "";

        for (Post p : this.posts) {
            if(!user.isCurrentlyIgnoring(p.getPoster()))
            result = result + p.render();
            if (--n < 0) break;
        }

        return result;
    }

     /**
     * Method to render n posts
     * @param n the ammount of posts to be rendered
     * @return a string with n posts     
     */
    
       private String render(int n) {
           String result = "";

        for (Post p : this.posts) {
            result = result + p.render();
            if (--n < 0) break;
        }

        return result;
    }

    /**
     * Method for printing out the feed for a user
     * uses the renderAll method 
     * @param user the account that is needed to check for ignored users in renderAll
     */
    
    public void printFeed(Account user) {  
        System.out.println(this.renderAll(user));
    }
}
