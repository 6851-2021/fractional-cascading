package fractional_cascading;

import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * 
 * @author isabellequaye
 *
 */
public class CatalogGraph<T>{
     
    private Set<Node> nodes;
    private Set<Edge> edges;
    
    
    public CatalogGraph(Map<T,List<Record>> nodes, Map<T,List<List<Integer>>> edges) {
        
    }
    
    public Set<Node> getNodes() {
        return Set.of();
    }
    
    public Set<Edge> getEdges() {
        return Set.of();
    }
}
