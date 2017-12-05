import java.io.Serializable;

/**
 * Class for the message of type AddFriend between the server and client
 * Takes an input of a friend that should be added and gives that message to the other party 
 */

public class AddFriend implements Serializable {
    private Account friendToBeAdded;

    /**
     * Method for adding the account that should be added to the message
     * @param friendToBeAdded the account that should be befriended
     */
    
    public AddFriend(Account friendToBeAdded) {
        this.friendToBeAdded = friendToBeAdded;
    }

    /**
     * Method for getting the account that should be added from the message
     * @return the account that should be befriended
     */
    
    public Account getFriend() {
        return this.friendToBeAdded;
    }
}
