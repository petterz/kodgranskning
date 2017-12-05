/**
 * Class holding the structure of a logout message between the server and client
 * Holds an account that will be transmitted
 * Hold methods for adding and taking an account from the message 
 */

public class Logout implements java.io.Serializable {
    private Account account;

    /**
     * Method for adding an account to the logout message
     * @param a the account that should be added
     */
    
    public Logout(Account a) {
        this.account = a;
    }

    /**
     * Method for takeing the account out from the message
     * @return the account that is stored in the message
     */
    
    public Account getAccount() {
        return this.account;
    }
}
