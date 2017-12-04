import java.util.List;
import java.util.LinkedList;
import java.io.*;
import java.net.*;
import java.util.*;


public class Feed {
    private List<Post> posts = new LinkedList<Post>();

    public void updateFeed(Set<Account> knownusers){
        Iterator<Account> it;
        for(it = knownusers.iterator(); it.hasNext(); ){
        Account knownUser = it.next();
        for (int i = 0; i < posts.size(); i++) {
            Post currentPost = posts.get(i);
            Account postUser = currentPost.getPoster();
            if(knownUser.getUserId()==postUser.getUserId()){
            postUser.setName(knownUser.getName());//HÄÄÄÄRRRRRRRR
                }
            }
        }
    }
    
    public void addPost(Post post) {
        posts.add(0, post);
    }

    public String renderAll() {
        return this.render(posts.size());
    }

    public String renderLatest(int n) {
        return this.render(n);
    }

    private String render(int n) {
        String result = "";

        for (Post p : this.posts) {
            result = result + p.render();
            if (--n < 0) break;
        }

        return result;
    }

    public void printFeed(){
        for(Post p :this.posts) {
            System.out.println(p.render());//render all gör samma sak!
        }
    }
}
