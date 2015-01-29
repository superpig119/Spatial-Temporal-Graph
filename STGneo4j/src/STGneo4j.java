import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStreamReader;
//import java.util.Collection;
//import java.util.Iterator;
//import java.util.List;
//import java.util.Queue;
import java.sql.PseudoColumnUsage;
import java.util.Scanner;
import java.util.Vector;
import java.util.concurrent.TimeUnit;
import java.util.*;

import org.neo4j.cypher.internal.compiler.v2_1.docbuilders.internalDocBuilder;
import org.neo4j.graphdb.Direction;
import org.neo4j.graphdb.DynamicLabel;
import org.neo4j.graphdb.GraphDatabaseService;
import org.neo4j.graphdb.Node;
import org.neo4j.graphdb.Label;
import org.neo4j.graphdb.schema.Schema;
import org.neo4j.graphdb.Relationship;
import org.neo4j.graphdb.RelationshipType;
import org.neo4j.graphdb.Transaction;
import org.neo4j.graphdb.factory.GraphDatabaseFactory;
import org.neo4j.graphdb.schema.IndexDefinition;
import org.neo4j.tooling.GlobalGraphOperations;
import org.neo4j.graphdb.ResourceIterator;

import scala.collection.mutable.LinkedList;

public class STGneo4j
{
	/**
	 * @param args
	 */
	private static final String DB_PATH = "/home/thor/downloads/neo4j-community-2.1.6/";
	private static final String DATA_PATH = "/home/thor/work/spatio-temporal-graph/data/data";
	
	GraphDatabaseService graphDb;
	IndexDefinition indexDefinition;
	private static enum RelTypes implements RelationshipType
	{
		TEMPOLINK
	}

	
	public static void main(String[] args)
	{
		// TODO Auto-generated method stub
		STGneo4j STG = new STGneo4j();
		STG.createDb();
		STG.SSSTGReachable(30, 500, 20, 1000);
		STG.removeData();
		STG.shutDown();
	}
	
	void createDb() 
	{
		graphDb = new GraphDatabaseFactory().newEmbeddedDatabase(DB_PATH);
		registerShutdownHook(graphDb);
		
		Label l = DynamicLabel.label("STG");
		//IndexManager index = graphDb.index();
		//Index<Node> inodes = index.forNodes("nodes");
		try(Transaction tx = graphDb.beginTx())
		{
			Schema schema = graphDb.schema();
			for (IndexDefinition indexDefinition : schema.getIndexes(l))
			{
				indexDefinition.drop();
			}
			indexDefinition = schema.indexFor(l).on("ID").create();
			//schema.awaitIndexOnline( indexDefinition, 100, TimeUnit.SECONDS );
			//Label label = DynamicLabel.Label("STG");
			tx.success();
		} 
		
		try(Transaction tx = graphDb.beginTx())
		{
			Schema schema = graphDb.schema();
			//Label l = DynamicLabel.label("STG");
			IndexDefinition indexDefinition = schema.getIndexes(l).iterator().next();
			while(schema.getIndexState(indexDefinition) != Schema.IndexState.ONLINE)
			{
				System.out.println("Waiting for the index");
				schema.awaitIndexOnline( indexDefinition, 1, TimeUnit.SECONDS );
			}
			System.out.println("Index is online!");
		}
		
		try(Transaction tx = graphDb.beginTx())
		{
			Schema schema = graphDb.schema();
			File filename = new File(DATA_PATH);
			InputStreamReader reader = new InputStreamReader(new FileInputStream(filename)); 
			//BufferedReader br = new BufferedReader(reader);
			Scanner in = new Scanner(reader);
			
			String line = "";
			int nodeNum, nodeID, nPos, nodeID2;
			nodeNum = in.nextInt();
			int i, j;
		
			for(i = 0; i < nodeNum; i++)
			{
				Node n = graphDb.createNode();
				nodeID = in.nextInt();
				n.setProperty("ID", nodeID);
				nPos = in.nextInt();
			//	Vector<nodeInfo> v = new Vector<nodeInfo>();
				int[] x = new int[nPos];
				int[] y = new int[nPos];
				int[] t1 = new int[nPos];
				int[] t2 = new int[nPos];
				for(j = 0; j < nPos; j++)
				{
					nodeInfo ni = new nodeInfo();
					x[j] = in.nextInt();
					y[j] = in.nextInt();
					t1[j] = in.nextInt();
					t2[j] = in.nextInt();
				}
				n.setProperty("x", x);
				n.setProperty("y", y);
				n.setProperty("t1", t1);
				n.setProperty("t2", t2);
				n.addLabel(l);
				//inodes.add(n, "ID", nodeID);
			}
			//System.out.println(nodeNum);
			
			int t1, t2;
			Relationship r;
			
			for(i = 0; i < nodeNum; i++)
			{
				nodeID = in.nextInt();
				nPos = in.nextInt();
				//IndexHits<Node> hits1 = inodes.get("ID", nodeID);
				//Node n1Node = hits1.getSingle();
				ResourceIterator<Node> rnodes = graphDb.findNodesByLabelAndProperty(l, "ID",  nodeID).iterator();
				Node n1Node = rnodes.next();
				for (j = 0; j < nPos; j++)
				{
					nodeID2 = in.nextInt();
					t1 = in.nextInt();
					t2 = in.nextInt();
					//IndexHits<Node> hits2 = inodes.get("ID", nodeID2);
					//Node n2Node = hits2.getSingle();
					ResourceIterator<Node> rnodes2 = graphDb.findNodesByLabelAndProperty(l, "ID",  nodeID2).iterator();
					Node n2Node = rnodes2.next();
					//System.out.println(hits1.getSingle().getProperty("ID"));
					//System.out.println(hits2.getSingle().getProperty("ID"));
					//r = hits1.getSingle().createRelationshipTo(hits2.getSingle(),RelTypes.TEMPOLINK);
					r = n1Node.createRelationshipTo(n2Node, RelTypes.TEMPOLINK);
					r.setProperty("t1", t1);
					r.setProperty("t2", t2);
				}
			}
			tx.success();
		} catch (FileNotFoundException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
	}
	
	private myPosition nodePos(Node n, int t)
	{
		myPosition pos = new myPosition();
		int[] x = (int[])n.getProperty("x");
		int[] y = (int[])n.getProperty("y");
		int[] t1 = (int[])n.getProperty("t1");
		int[] t2 = (int[])n.getProperty("t2");
		int i;
		for(i = 0; i < x.length; i++)
		{
			if (t >= t1[i] && t <= t2[i] && i + 1 < x.length)
			{
				pos.x = x[i] + (x[i+1] - x[i]) * (t + i - t1[i]) / (t2[i] - t1[i]);
				pos.y = y[i] + (y[i+1] - y[i]) * (t + i - t1[i]) / (t2[i] - t1[i]);
				return pos;
			}
		}
		pos.x = x[x.length-1];
		pos.y = y[y.length-1];
		return pos;
	}
	
	private int nodeDist(Node n1, Node n2, int t)
	{
		myPosition p1, p2;
		p1 = nodePos(n1, t);
		p2 = nodePos(n2, t);
		return (int)Math.sqrt(Math.pow(p2.x - p1.x, 2) + Math.pow(p2.y - p1.y, 2));
	}
	
	private int SpatialReachable(Node n1, Node n2, int esp, int t1, int t2)	//return the earliest time from t1 to t2 that n1 and n2 are spatially reachable
	{
		int t = t2 - t1;
		int dist;
		int i;
		for (i = 0; i < t; i++)
		{
			dist = nodeDist(n1, n2, t1 + i);
			if (dist <= esp)
			{
				return t1 + i;
			}
		}
		return -1;
	}
	
	void SSSTGReachable(int nodeID, int esp, int t1, int t2)//single source STG Reachability
	{
		try(Transaction tx = graphDb.beginTx())
		{
			Label l = DynamicLabel.label("STG");
			ResourceIterator<Node> rnodes = graphDb.findNodesByLabelAndProperty(l, "ID",  nodeID).iterator();
			Node sourceNode = rnodes.next();
			Vector<PathInfo> vPathInfo = new Vector<PathInfo>();
			//Queue<PathInfo> qPath = new LinkedList<PathInfo>();
			//LinkedList<PathInfo> qPath = new LinkedList<PathInfo>();
			Queue qPath = new java.util.LinkedList();
			PathInfo psource = new PathInfo();
			psource.node = sourceNode;
			psource.id = (int)sourceNode.getProperty("ID");
			psource.t1 = t1;
			psource.t2 = t2;
			qPath.add(psource);
			vPathInfo.add(psource);
			Map<Integer, Boolean> mnodeVisited = new HashMap<Integer, Boolean>();
			for(Node node: GlobalGraphOperations.at(graphDb).getAllNodes())
			{
				if(node.hasProperty("ID") && node.hasProperty("x"))
				{
					mnodeVisited.put((int)node.getProperty("ID"), false);
				}
			}
			
			while(!qPath.isEmpty())
			{
				PathInfo p = (PathInfo) qPath.remove();
				Node n = p.node;
				mnodeVisited.put((int)n.getProperty("ID"), true);
				for(Relationship r : n.getRelationships())
				{
					if(!mnodeVisited.get((int)r.getEndNode().getProperty("ID")))
					{
						if(p.t2 < (int)r.getProperty("t1") || p.t1 > (int)r.getProperty("t2"))
							continue;
						else 
						{
							PathInfo p2 = new PathInfo();
							p2.id = (int)r.getEndNode().getProperty("ID");
							if(p2.id == 602)
							{
								System.out.print("602");
							}
							p2.node = r.getEndNode();
							p2.t1 = Math.max(p.t1, (int)r.getProperty("t1"));
							p2.t2 = p.t2;
							int t = SpatialReachable(n, p2.node, esp, Math.max(p.t1, (int)r.getProperty("t1")), t2);
							if(t != -1 && t < t2)
							{
								p2.t1 = t;
								qPath.add(p2);
								vPathInfo.add(p2);
								mnodeVisited.put(p2.id, true);
								System.out.println("From " +  n.getProperty("ID").toString() + " to " + p2.id + " t:" + t + " edge info:rt1:" + (int)r.getProperty("t1") + " rt2:" + (int)r.getProperty("t2"));
							}
						}
						//Node n2 = r.getEndNode();
						
						/*Check the spaitial part
						 * 
						 */
					}
				}
			}
			
			System.out.println("Temporal reachable nodes:");
			for (PathInfo p : vPathInfo)
			{
				System.out.format("%d from %d to %d\n", p.id, p.t1, p.t2);
			}
			
			tx.success();
		}
	}
	
	void removeData()
	{
		try(Transaction tx = graphDb.beginTx())
		{
			Schema schema = graphDb.schema();
			Label l = DynamicLabel.label("STG");
			IndexDefinition indexDefinition = schema.getIndexes(l).iterator().next();
			while(schema.getIndexState(indexDefinition) != Schema.IndexState.ONLINE)
			{
				System.out.println("Waiting for the index");
				schema.awaitIndexOnline( indexDefinition, 5, TimeUnit.SECONDS );
			}
			System.out.println("Index is online!");
		}
		
		Transaction tx = graphDb.beginTx();
		try
		{
			System.out.println("Deleting the Graph");
		//	for(String s : GlobalGraphOperations.at(graphDb).getAllPropertyKeys())
			//	System.out.println(s);
			//GlobalGraphOperations graphOperations = GlobalGraphOperations.at(graphDb);
			//Iterable<Node> nodes = graphOperations.getAllNodes();
			for(Node node: GlobalGraphOperations.at(graphDb).getAllNodes())
			{
				if(node.hasProperty("ID") && node.hasProperty("x"))
				{
					//System.out.println(node.getProperty("ID").toString());
					int[] x,y,t1,t2;
					x = (int[])node.getProperty("x");
					y = (int[])node.getProperty("y");
					t1 = (int[])node.getProperty("t1");
					t2 = (int[])node.getProperty("t2");
					//System.out.format("%d\t%d\t%d\t%d\n", x[0], y[0], t1[0], t2[0]);
					for(Relationship rr :node.getRelationships(RelTypes.TEMPOLINK, Direction.OUTGOING))
					{
						//int ts = (int)rr.getProperty("t1");
						//int te = (int)rr.getProperty("t2");
						//System.out.format("%d to %d from %d to %d\n", (int)node.getProperty("ID"), (int)rr.getEndNode().getProperty("ID"), ts, te);
						rr.delete();
					}
					for(Relationship rr :node.getRelationships(RelTypes.TEMPOLINK, Direction.INCOMING))
					{
					//	System.out.format("%d to %d from %d to %d\n", (int)rr.getStartNode().getProperty("ID"), (int)node.getProperty("ID") , ts, te);
						rr.delete();
					}
					node.delete();
				}
			}
			System.out.println("The Graph is deleted!");
			tx.success();
		} finally
		{
			tx.close();
		}
	}
	
	private static void registerShutdownHook(final GraphDatabaseService graphDb)
	{
		Runtime.getRuntime().addShutdownHook(new Thread()
		{
			@Override
			public void run()
			{
				graphDb.shutdown();
				System.out.println("Used shut down hook.");
			}
		});
	}
	void shutDown()
	{
		graphDb.shutdown();
		System.out.println("graphDB shut down.");
	}
}



