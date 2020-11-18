package parallelTree;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;

public class ReadTreePart implements Runnable {
	TreeNode tree;
	String fileName;
	Semaphore sem;
	public ReadTreePart(TreeNode tree, String fileName, Semaphore sem) {
		this.tree = tree;
		this.fileName = fileName;
		this.sem = sem;
	}

	@Override
	public void run() {
		try {
			Scanner scanner = new Scanner(new File(fileName));
			TreeNode treeNode;

			while (scanner.hasNextInt()) {
				int child = scanner.nextInt();
				int root = scanner.nextInt();

				treeNode = tree.getNode(root);
				while (treeNode == null) {
					treeNode = tree.getNode(root);
				}
				synchronized (tree) {
					treeNode.addChild(new TreeNode(child));
				}
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		sem.release();
	}
}
