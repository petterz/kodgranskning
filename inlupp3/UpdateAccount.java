import java.io.Serializable;

/**
 * Class for the strucret uof a message of type UpdateAccount between the server and the client
 * Holds methods for adding and extracting accounts to the message
 */

public class UpdateAccount implements Serializable {
    private Account oldAccount;
    private Account newAccount;

    /** 
     * Method for adding the old information about the account to the message
     * @param old the old account information
     */
    
    public void AddOldAccount(Account old) {
        this.oldAccount=old;
    }

    /** 
     * Method for adding the new information about the account to the message
     * @param newAccount the new account information
     */
    
    public void AddNewAccount(Account newAccount) {
        this.newAccount=newAccount;
    }

    /**
     * Method for extracting the old account information from the message
     * @return the old account
     */
    
    public Account getOldAccount() {
        return this.oldAccount;
    }

     /**
     * Method for extracting the new account information from the message
     * @return the new account
     */
    
    public Account getNewAccount() {
        return this.newAccount;
    }
}
