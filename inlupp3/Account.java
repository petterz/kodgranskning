import java.util.Set;
import java.util.TreeSet;
import java.io.Serializable;

/**
 * Class containing the structure for an account and methods to set and get the information
 * Holds the users name, password, userId aswell as a list of friends and ignored friends
 */

public class Account implements Serializable, Comparable<Account> {
    private String name;
    private String password;
    private String userId;
    private boolean passwordReset;
    private Set<Account> friends = new TreeSet<Account>();
    private Set<Account> ignoredFriends = new TreeSet<Account>();
    private int postsAtLastSync;  //keeps track of how far the feed from the server is synced

    /**
     * Constructs an account without a name good for comparing if the user exists 
     * @param userId the users unique id which is used to kep track on the account
     * @param password the password for the user
     */
    
    public Account(String userId, String password) {
        this.userId   = userId;
        this.password = password;
    }

    /**
     * Constructs an account with complete information
     * Used when you encounter a new user
     * @param userId the users unique id which is used to kep track on the account
     * @param password the password for the user
     * @param name the name that is displayed to the uther users
     */
    
    public Account(String userId, String password, String name) {
        this(userId, password);
        this.name = name;
    }

    /**
     * Gets the information from postsAtLastSync about how far the client is synced with the server
     * @return how many posts from the server feed that has already been syncronized
     */
    
    public int getPostAtLastSync() {
        return this.postsAtLastSync;
    }

    /**
     * Sets a new amount of syncronized posts for postsAtLastSync 
     * @param posts the amount of posts that has been syncronized
     */
    
    public void setPostAtLastSync(int posts) {
        this.postsAtLastSync = posts;
    }

    /**
     * Gets the name of the account
     * @return the name of the account
     */
    
    public String getName() {
        return this.name;
    }

    /**
     * Method the set a new name for the account
     * @param name the new name for the account
     */
    
    public void setName(String name) {
        this.name = name;
    }

    /**
     * Method to get an accounts password
     * @return the password of the account
     */
    
    public String getPassword() {
        return this.password;
    }

    /**
     * Method to set a new password for the account
     * @param password the new password for the account
     */
    
    public void setPassword(String password) {
        this.password = password;
    }

    /**
     * Method to get an accounts user ID
     * @return the accounts userId
     */
    
    public String getUserId() {
        return this.userId;
    }

    /**
     * Method to set an accounts user ID
     * @param userId the user ID that we want to link to the account
     */
    
    public void setUserId(String userId) {
        this.userId = userId;
    }

    /**
     * Method for adding a user to he friendlist
     * @param a the account that should be added to the friendlist
     */
    
    public void addFriend(Account a) {
        this.friends.add(a);
    }

    /**
     * Method for removing a friend from the friendlist
     * @param a the friend that should be removed 
     */
    
    public void removeFriend(Account a) {
        this.friends.remove(a);
    }

    /**
     * Method for adding a current friend to the ignnored list
     * @param a the friend that should be ignored
     */
    
    public void ignoreFriend(Account a) {
        if (this.isFriendsWith(a)) this.ignoredFriends.add(a);
    }

    /**
     * Method for unignoring a friend from the ignored list
     * @param a the friend that should be unignored
     */
    
    public void unIgnoreFriend(Account a) {
        if (this.isFriendsWith(a)) this.ignoredFriends.remove(a);
    }

    /**
     * Method to see if an account is friends with the user
     * @param a the account that should be checked to see if the user is friends with it
     * @return returns true if the users are friends else false 
     */
    
    public boolean isFriendsWith(Account a) {
        return this.friends.contains(a);
    }

    /**
     * Method to see if the user is ignoring an account
     * @param a the account that we want to see if we ignore
     * @return true if the account is ignored else false
     */
    
    public boolean isCurrentlyIgnoring(Account a) {
        return this.ignoredFriends.contains(a);
    }

    /**
     * Method for comparing the users userId to another accounts 
     * @param a the account to compare against
     * @return = if the userIds are identical else -1 or 1 depending on thier order in ASCII
     */
    
    public int compareTo(Account a) {
        return a.userId.compareTo(this.userId);
    }

    /**
     * Method to see if the user has any friends
     * @return true if the friendlist isn't empty else false
     */
    
    public boolean hasFriends() {
        return this.friends.size() > 0;
    }

    /**
     * Method to get an accounts friendlist
     * @return returns the users friendlist as an array of accounts
     */
    
    public Account[] getFriends() {
        return (Account[]) this.friends.toArray(new Account[0]);
    }

    /**
     * Method to get the users ignored friends
     * @return returns the list of ignored friends as an array of accounts
     */
    
    public Account[] getIgnoredFriends() {
        return (Account[]) this.ignoredFriends.toArray(new Account[0]);
    }

    /**
     * Method to see if an object is the same account as the user
     * It's used when transfering messages between the server and client
     * @param o the object to be evaluated
     * @return true if the object is the same as the account of the user else false
     */
    
    public boolean equals(Object o) {
        if (o instanceof Account) {
            return ((Account) o).userId.equals(this.userId);
        } else {
            return false;
        }
    }
}
