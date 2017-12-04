import java.util.*;
import java.io.*;
import java.util.Iterator;
import java.net.*;

public class Server {
    private Set<Account> knownUsers = new TreeSet<Account>();
    private Set<Account> safeAccounts = new TreeSet<Account>();
    private List<Post> posts = new LinkedList<Post>();

    public static void main(String[] args) {
        try {
            ServerSocket socket = new ServerSocket(args.length > 0 ? Integer.parseInt(args[0]) : 8080);
            Server server = new Server();

            while (true) {
                System.out.println("!! Server listening for connections: " + socket.getInetAddress() + ":" + socket.getLocalPort());
                Socket clientConnection = socket.accept();
                System.out.println("!! Server got a connection from: " + clientConnection.getInetAddress() + ":" + clientConnection.getPort());
                try {
                    ClientProxy.attemptEstablishConnection(clientConnection, server);
                } catch (RuntimeException e) {
                    System.err.println(e.getMessage());
                }
            }
        } catch (IOException ioe) {
            ioe.printStackTrace();
        } catch (ClassNotFoundException cnfe) {
            cnfe.printStackTrace();
        }
    }

    public Account getAccountFor(String userId) {
        for (Account a : this.knownUsers)
            if (a.getUserId().equals(userId)) return a;

        return null;
    }

    public Account getAccountForName(String name) {
        for (Account a : this.knownUsers)
            if (a.getName().equals(name)) return a;

        return null;
    }

    public synchronized void addAccount(Account a) {
        this.knownUsers.add(a);
    }

    public synchronized void removeAccount(Account a) {
        this.knownUsers.remove(a);
    }

    public synchronized void addSafeAccount(Account a) {
        this.safeAccounts.add(a);
    }

    public synchronized void removeSafeAccount(Account a) {
        this.safeAccounts.remove(a);
    }
    
    public synchronized Set<Account> getAccounts() {
        return new TreeSet<Account>(this.knownUsers);
    }

    public synchronized Set<Account> getSafeAccounts(){
        this.updateSafeAccounts();
        return new TreeSet<Account>(this.safeAccounts);                      
    }
    public synchronized void updateSafeAccounts(){
           for (Account a : this.knownUsers){
            this.safeAccounts.add(a.safecopy());
            //   System.out.print("safecopy: " + (a.safecopy()).getName());
        }
    }

    /*
    public synchronized List<Post> getOldPosts() {
        return new ArrayList<Post>(this.posts);
    }
    */
     //public synchronized List<Post> getNewFriendPosts(Account account) {
/// Get all new posts from friends is a special case of get new posts
    public synchronized List<Post> getNewFriendPosts(Account account) {
        List<Post> result = new ArrayList<Post>();
    for (Post p : this.getNewPosts(account)) {
        if (account.isFriendsWith(p.getPoster())) result.add(p);
    }
    return result;
    }

    /// Read "time stamp" from account, then update it 
    public synchronized List<Post> getNewPosts(Account account) {
    int since = account.getPostAtLastSync();
    account.setPostAtLastSync(this.posts.size());

    return new ArrayList<Post>(this.posts.subList(since, this.posts.size()));
    }

    public synchronized void addPost(Post p) {
        this.posts.add(p);
    }

    static class ClientProxy extends Thread {
        private Account account;
        private Socket socket;
        private Server server;
        private ObjectOutputStream outgoing;
        private ObjectInputStream incoming;

        private ClientProxy(Account account, Socket socket, Server server, ObjectInputStream incoming) throws IOException {
            this.account = account;
            this.server  = server;
            this.socket  = socket;
            this.incoming = incoming;
            this.outgoing = new ObjectOutputStream(socket.getOutputStream());
            System.out.println("<< Account");
            this.outgoing.writeObject(account);
            this.outgoing.flush();
        }

        public static void attemptEstablishConnection(Socket socket, Server server) throws IOException, ClassNotFoundException {
            ObjectInputStream incoming = new ObjectInputStream(socket.getInputStream());
            Object handShake = incoming.readObject();

            if (handShake instanceof Login) {
                Account account = ((Login) handShake).getAccount();
                Account knownAccount = server.getAccountFor(account.getUserId());

                if (knownAccount == null) {
                    server.addAccount(account);
                    new ClientProxy(account, socket, server, incoming).start();
                } else {
                    if (knownAccount.getPassword().equals(account.getPassword()) == false) throw new RuntimeException("Wrong password");
                    new ClientProxy(knownAccount, socket, server, incoming).start();
                }
            } else {
                System.err.println("!! Bad connection attempt from: " + socket.getInetAddress() + ":" + socket.getPort());
            }
        }

        private int globalPostIdCounter = 0;
        // The synchronised keyword is required on all methods which may
        // be called in parallel on the server from multiple clients at
        // the same time.
        private synchronized int getUniqueGlobalPostId() {
            return ++this.globalPostIdCounter;
        }

        private void logout(Account a) {
            this.server.removeAccount(a);
            System.out.println("!! " + a.getUserId() + " left the building");
            try {
                this.outgoing.close();
                this.incoming.close();
            } catch (IOException ioe) {
                ioe.printStackTrace();
            }
        }

        private void postMessage(String msg) {
            //System.out.println("inne i post: " + this.account.getName());
            this.server.addPost(new Post(this.getUniqueGlobalPostId(), this.account, msg));
        }

        private void addFriend(Account a) {
            this.account.addFriend(a);
            a.addFriend(this.account);
        }

        private void removeFriend(Account a) {
            this.account.removeFriend(a);
            a.removeFriend(this.account);
        }

        public void loopOverKnownUsers(NameChange namechange){
            Account oldAcc = namechange.getOldName();
            Account newAcc = namechange.getNewName();
            Iterator<Account> it;
            for(it = server.safeAccounts.iterator(); it.hasNext(); ){
                Account next = it.next();
                // System.out.println("known user: " + next.getName());
                if(this.account.isFriendsWith(next)){
                    
                    next.changeFriendName(oldAcc, newAcc);//HÄR!
                    //      System.out.print("a friend " + this.account.getName() + "has changed his/her name, "+ next.getName());
                    next.printFriends();
                    //försvinner vår info utanför for-loopen?
                }
                else{
                    //System.out.println("inte vänner");

                } 
            }
           this.account.setName(newAcc.getName());
           //System.out.println("Först: " + this.account.getName() + " sedan : " + newAcc.getName());
           this.updateAccount(oldAcc, newAcc);
           this.updateSafeAccount(oldAcc, newAcc);
        }

         public void printKnownUsers(){
            Iterator<Account> it;
            for(it = server.knownUsers.iterator(); it.hasNext(); ){
                Account next = it.next();
                next.printFriends();
                }
        }
        
        private void updateAccount(Account old, Account newacc) {
            server.removeAccount(old);
            server.addAccount(newacc);
        }

        private void updateSafeAccount(Account old, Account newacc){
            server.removeSafeAccount(old);
            server.addSafeAccount(newacc);
        }

        private void sync() {
            this.printKnownUsers();
            try {
                System.out.println("<< SyncResponse, this.account:" + this.account.getName());
                this.outgoing.
                writeObject(new SyncResponse(new HashSet<Account>(this.server.getSafeAccounts()),
                                             new LinkedList<Post>(this.server.getNewFriendPosts(this.account))));
                this.outgoing.flush();
            } catch (IOException ioe) {
                ioe.printStackTrace();
            }
        }

        public void run() {
            try {
                while (true) {
                    this.printKnownUsers();
                   
                    Object o = this.incoming.readObject();
                    System.err.println(">> Received: " + o.getClass().getName());
                    // o instanceof Account checks if o is an account
                    // (Account) o type casts o into an Account so that it can be used as one
                    if (o instanceof Account) {
                       
                        this.updateAccount(this.account, (Account) o);
                    } else if (o instanceof PostMessage) {
                       
                        this.postMessage(((PostMessage) o).getMsg());
                    } else if (o instanceof AddFriend) {
                       
                        this.addFriend(((AddFriend) o).getFriend());
                    } else if (o instanceof RemoveFriend) {
                       
                        this.removeFriend(((RemoveFriend) o).getFriend());
                    } else if (o instanceof SyncRequest) {
                       
                        this.sync();
                    } else if (o instanceof Logout) {
                       
                        this.logout(((Logout) o).getAccount());
                        return;
                    } else if (o instanceof NameChange){

                        NameChange namechange = (NameChange) o;
                         this.loopOverKnownUsers(namechange);
                        //this.changeFriendName(namechange);
                    //casta vårt namechabnge ocg ändra namnet för alla typ?
                    }
                }
            } catch (Exception e) {
                // BAD Practise. Never catch "Exception"s. Too general.
                e.printStackTrace();
            }
        }
    }
}

