import java.io.Serializable;

/**
 * Class holding the structure of a message between the server and client of type remove friend
 * Holds methods for adding and extracting the account that should be removed as friend
 */

public class RemoveFriend implements Serializable {
    private Account friendToBeRemoved;

    /**
     * Method for adding the account to the message
     * @param friendToBeRemoved the account that should be added to the message
     */
    
    public RemoveFriend(Account friendToBeRemoved) {
        this.friendToBeRemoved = friendToBeRemoved;
    }

    /**
     * Method for extracting the account from the message
     * @return the account that was included in the message
     */
    
    public Account getFriend() {
        return this.friendToBeRemoved;
    }
}
