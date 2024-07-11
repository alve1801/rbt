#include<stdio.h>

struct rbnode{
	bool color;
	int value; // idk, something to sort by for convenience
	rbnode*p,*c[2];

	rbnode(int val,rbnode*p):value(val),color(1),p(p){c[0]=c[1]=0;}

	bool insert(int i){
		bool r=i>value;
		rbnode*at=c[r];
		if(!at){
			c[r]=new rbnode(i,this);
			return color; // if we are red, theres a double-red situation that the parent must resolve
		}
		if(!at->insert(i))return 0; // recurses, and returns if no errors

		if(!color && c[!r] && c[!r]->color){
			// black parent of two red kids, one of which is parent of a red kid
			c[0]->color=c[1]->color=0,color=1;return 1;
		}

		// if children are in different directions
		if(r != i>at->value)c[r]=at->rotate(i>at->value);

		// two children in a row - rotate w/ current
		color=1;at->color=0;
		if(!p){rotate(!r);return 1;}

		p->c[i>p->value]=rotate(!r);
		return 0;
	}

	rbnode*rotate(bool dir){
		// returns whatever is considered the new root of the tree (makes indexing easier)
		bool rid=!dir;
		rbnode*ret=c[rid]; // child of root is b
		c[rid]->p=p; // parent of b is root
		p=c[rid]; // b is parent of a
		c[rid]=p->c[dir]; // d is child of a
		if(c[rid])c[rid]->p=this; // a is parent of d
		p->c[dir]=this; // a is child of b
		return ret;
	}

	void print(){
		printf("(%i%c ",value,color?'r':'b');
		if(c[0])c[0]->print();else putchar('.');
		if(c[1])c[1]->print();else putchar('.');
		putchar(')');
	}

	void draw(int depth=0){
		if(c[0])c[0]->draw(depth+1);
		for(int i=0;i<depth;i++)putchar(32);
		printf("%i%c %c%c\n",value,color?'r':'b',c[0]?'#':'.',c[1]?'#':'.');
		if(c[1])c[1]->draw(depth+1);
	}
};


int main(){
	rbnode*tree=new rbnode(0,0);
	tree->print();putchar(10);
	for(int i=1;i<30;i++){
		tree->insert(i);
		if(tree->p)tree=tree->p;
		tree->print();
		putchar(10);
		putchar(10);
	}

	tree->draw();
}

/*
the rope tree will suffer from balancing issues, do we wanna make it a red-black tree?
this is gonna need an INSANE amount of code and integration, but it WOULD work
https://brilliant.org/wiki/red-black-tree/
- redblack trees have an extra bit to store color
- root and leaves are always black, inserts always red
- red nodes cannot have red children
when inserting, if the parent of the inserted node is also red, tree has to be rebalanced.
if the uncle (technical term) is red, recolor parent and uncle to black, grandparent to red, and continue checking from grandparent
if black and current is left child of left child (or vice versa), perform right rotation then recolor parent and grandparent
if black and current is right of left (or vv), only perform right rotate
when removing a black node or recoloring a red node, we can get a double-black situation (which sounds very racist)
if sibling is red, recolor sibling and parent and rotate (which direction?)
if sibling has a red child, rotate and recolor (which?)
else recolor sibling to red and move problem to parent
left rotation:
 x      y_
  y -> x  b
 a b    a
overall, it seems like we might wanna just take the conditions and figure shit out on paper or smth
... were using two very sophisticated structures, would a design pattern be helpful here?

---
           0b
         4r
     6b      2b
  8b   5b  3b  1b
9r  7r

... i mean it mostly works? no idea why root's fucked tho
fixing it would require some way to change the root tho
also we are currently sorting in the wrong direction lol

  0b
    4r
 2b    6b
1b 3b 5b  8b
         7r 9r

trying to rotate root would result in assigning to nullpointer
we could also just... rotate regularly, and return a 1 to signify we should do tree=tree->p
still very ugly, but hey if it works
hm. now it manages to make a circular link.
at second cycle (first one that needs readjustment), the root's left child is itself

... so, it actually seems to do everything correctly
is it the reassignment in main that effs it up?
it could be that it tries to overwrite the object (instead of just changing the reference), and thereby effs up the process. should not happen if we use a pointer in main - try that out?
huh. well, that kinda works. doesnt properly report when we should change it, but otherwise

    3r
  1b    7b
0b 2b  5r  8b
     4b 6b  9r

looking good? root is red, but we dont really care abt that tbh. try more numbers and see if it holds?

tried for up to a hundred and it seemed to work, though the nodes were suspiciously black...
ah well, if it works. now, off to expanding them to ropes!
actually first we might wanna simplify them some

--

correction: we should also (probably) implement removals
- if we have two children, find leftmost node in right tree, and replace current element w/ that, then delete it (recursive) (i think we color it black afterwards?)
- if we only have one, replace w/ that (and color it black)
- if no children and red, simply delete
- if no children and BLACK, then we have issues. none of the papers are very useful in figuring out how to solve them though, so maybe work it out by hand?
https://en.wikipedia.org/wiki/Red%E2%80%93black_tree#Removal
https://www.geeksforgeeks.org/introduction-to-red-black-tree/#2-deletion
https://www.programiz.com/dsa/deletion-from-a-red-black-tree

*/
