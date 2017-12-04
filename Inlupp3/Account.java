import java.util.Set;
import java.util.Iterator;
import java.util.TreeSet;
import java.io.Serializable;

public class Account implements Serializable, Comparable<Account> {
    private String name;
    private String password;
    private String userId;
    private boolean passwordReset;
    private Set<Account> friends = new TreeSet<Account>();
    private Set<Account> ignoredFriends = new TreeSet<Account>();

    public Account(String userId, String password) {
        this.userId   = userId;
        this.password = password;
    }

    public Account(String userId, String password, String name) {
        this(userId, password);
        this.name = name;
    }

    public String getName() {
        return this.name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public String getPassword() {
        return this.password;
    }
    public void setPassword(String password) {
        this.password = password;
    }
    public String getUserId() {
        return this.userId;
    }
    public void setUserId(String userId) {
        this.userId = userId;
    }

    private int postsAtLastSync; 

    public int getPostAtLastSync() {
    return this.postsAtLastSync;
    }

    public void setPostAtLastSync(int posts) {
    this.postsAtLastSync = posts;
    }

    public void changeFriendName(Account oldAcc, Account newAcc){
        Set<Account> friends = this.friends;
        Iterator<Account> it;
        for (it = friends.iterator(); it.hasNext(); ) {
            Account f = it.next();
            String fname = f.getName();
            if (fname.equals(oldAcc.getName())){
                this.removeFriend(oldAcc);
                this.addFriend(newAcc);
                break;
            }
            else{
            }
        }
    }
    public void changeInAllFriends(Account oldAcc, Account newAcc){
        Set<Account> friends = this.friends;
        Iterator<Account> it;
        for (it = friends.iterator(); it.hasNext(); ) {
            Account f = it.next();
            f.changeFriendName(oldAcc, newAcc);
        }
    }
    public void addFriend(Account a) {
        this.friends.add(a);
    }

    public void removeFriend(Account a) {
        this.friends.remove(a);
    }

    public void ignoreFriend(Account a) {
        if (this.isFriendsWith(a)) this.ignoredFriends.add(a);
    }

    public void unIgnoreFriend(Account a) {
        if (this.isFriendsWith(a)) this.ignoredFriends.remove(a);
    }

    public boolean isFriendsWith(Account a) {
        return this.friends.contains(a);
    }

    public boolean isCurrentlyIgnoring(Account a) {
        return this.ignoredFriends.contains(a);
    }

    public int compareTo(Account a) {
        return a.userId.compareTo(this.userId);
    }

    public boolean hasFriends() {
        return this.friends.size() > 0;
    }

    public Account[] getFriends() {
        return (Account[]) this.friends.toArray(new Account[0]);
    }

    public void printFriends() {
        Set<Account> friends = this.friends;
        Iterator<Account> it;
        for (it = friends.iterator(); it.hasNext(); ) {
            Account f = it.next();
            System.out.println(this.getName() + " har vännen:" + f.getName());
    }
    }

    public Account safecopy(){
    Account result = new Account(this.userId, "hemligt", this.name);
    result.passwordReset = this.passwordReset;
    result.friends = this.friends;
    result.ignoredFriends = this.ignoredFriends;
    return result;
    }

    public Account newNameCopy(String name){
    Account result = new Account(this.userId, this.password, name);
    result.passwordReset = this.passwordReset;
    result.friends = this.friends;
    result.ignoredFriends = this.ignoredFriends;
    return result;
    }
    
    public Account[] getIgnoredFriends() {
        return (Account[]) this.ignoredFriends.toArray(new Account[0]);
    }

    public boolean equals(Object o) {
        if (o instanceof Account) {
            return ((Account) o).userId.equals(this.userId);
        } else {
            return false;
        }
    }
}
