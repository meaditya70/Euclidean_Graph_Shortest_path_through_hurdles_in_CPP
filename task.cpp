/*

The task if to find a shortest path from a source 's' to a destination 't' in an Euclidean
graph. 

--------------------------------------------

Compilation Instruction: g++ task.cpp
Execution Instruction: ./a.out <k> <m> <n> <g>

E.g.: 
 >>>>> g++ task.cpp
 >>>>> ./a.out 2 20 30 10

 -------------------------------------------

 Additional Information:

 1. Kindly provide the value of m, n and g greater than 0.
 2. The value of k should be less than or equal to (m*n)/(g*g)

Voilation of above two points will print invalid output on the terminal.

Also, Please keep an upperbound to the total number of points(((m/g)+1)*((n/g)+1)) upto 1000.
E.g.: m = 300; n=300; q=10; has 961 points and it works fine.
------------------------------------------------------


*/




#include<iostream>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<climits>
#include<cmath>
using namespace std;



//defining a structure for a point having X and Y coordinates
typedef struct point{
	double x;
	double y;
}Coordinates;


//to obtain the value of y-coord given x-coord and a line
double get_y(double x1,double y1,double x2,double y2,double x){
	double slope=(y2-y1)/(x2-x1);
	return slope*(x-x1)+y1;
}

//to obtain the value of x-coord given y-coord and a line
double get_x(double x1,double y1,double x2,double y2,double y){
	double slope=(y2-y1)/(x2-x1);
	return (y-y1)/slope+x1;
}


//used to calculate the euclidean distance between two points (edge-weights here)
double distance(double x1,double y1,double x2,double y2,int q){
	double delta_xsq=pow((x2-x1),2);
	double delta_ysq=pow((y2-y1),2);
	double result = sqrt(delta_xsq+delta_ysq);
	if(q!=0) result = result/q;
	return result;
}


//to check if an inclined line passes through an obstacle 
int isclear(Coordinates *point,int i, int j,int *hurdles_position, int num_points,int q){
	double minX,minY,maxX,maxY; // to get the boundary region of the edge from i to j
	if(point[i].x < point[j].x){
		minX=point[i].x;
		maxX=point[j].x;
	}
	else{
		minX=point[j].x;
		maxX=point[i].x;
	}
	if(point[i].y < point[j].y){
		minY=point[i].y;
		maxY=point[j].y;
	}
	else{
		minY=point[j].y;
		maxY=point[i].y;
	}

	for(int k=0;k<num_points;k++){ 
	//iterating over all points but checking only the points that is an obstacle in the boundary region obtained above 
		if(point[k].x>=minX && point[k].x<=maxX && point[k].y>=minY && point[k].y<=maxY){
			if(hurdles_position[k]==1){

				//for checking the edge(i,j) lies within an obstacle(end points of e(i,j) diagonally at vertices)
				int flag=0;
				if((point[k].x==point[i].x && point[k].y==point[i].y) || (point[k].x==point[j].x && point[k].y==point[j].y)){
					flag++;
				}
				if((point[k].x+q==point[i].x && point[k].y==point[i].y) || (point[k].x+q==point[j].x && point[k].y==point[j].y)){
					flag++;
				}
				if((point[k].x==point[i].x && point[k].y+q==point[i].y) || (point[k].x==point[j].x && point[k].y+q==point[j].y)){
					flag++;
				}
				if((point[k].x+q==point[i].x && point[k].y+q==point[i].y) || (point[k].x+q==point[j].x && point[k].y+q==point[j].y)){
					flag++;
				}
				if(flag==2) return 0;



				//for checking the egde(i,j) going through the obstacle(through the edges of the obstacles, not vertices)
				int cases=4;
				while(cases){
					double x_new,y_new;
					if(cases==1){ //right edge of the obstacle
						y_new=point[k].y+q;
						x_new=get_x(point[i].x,point[i].y,point[j].x,point[j].y,(int)y_new);
						if(x_new>point[k].x && x_new<point[k].x+q){
							return 0;
						}
					} 
					else if(cases==2){ //bottom edge of the obstacle
						x_new=point[k].x+q;
						y_new=get_y(point[i].x,point[i].y,point[j].x,point[j].y,(int)x_new);
						if(y_new>point[k].y && y_new<point[k].y+q){
							return 0;
						}
					}
					else if(cases==3){ //left edge of the obstacle
						y_new=point[k].y;
						x_new=get_x(point[i].x,point[i].y,point[j].x,point[j].y,(int)y_new);
						if(x_new>point[k].x && x_new<point[k].x+q){
							return 0;
						}
					}
					else if(cases==4){ //top edge of the obstacle
						x_new=point[k].x;
						y_new=get_y(point[i].x,point[i].y,point[j].x,point[j].y,(int)x_new);
						if(y_new>point[k].y && y_new<point[k].y+q){
							return 0;
						}
					}
					cases--;
				}
				
				//for checking the edge(i,j) passes through an obstacle(diagonally through the vertices,not edges)
				double hurdles_points[4][2]={{point[k].x,point[k].y},{point[k].x+q,point[k].y+q},{point[k].x+q,point[k].y},{point[k].x,point[k].y+q}};
				for(int indx=0;indx<4;indx+=2){
					double x_new=hurdles_points[indx][0];
					double y_new=hurdles_points[indx][1];
					double x_new1=hurdles_points[indx+1][0];
					double y_new1=hurdles_points[indx+1][1];
					
					double dist=distance(point[i].x,point[i].y,point[j].x,point[j].y,q);
					if(y_new==get_y(point[i].x,point[i].y,point[j].x,point[j].y,(int)x_new) && y_new1==get_y(point[i].x,point[i].y,point[j].x,point[j].y,(int)x_new1)){
						if(dist>distance(point[i].x,point[i].y,x_new,y_new,q)) return 0;
						if(dist>distance(point[i].x,point[i].y,x_new1,y_new1,q)) return 0;
					}
				}
				
				
			}
		}
	}


	return 1;
}

//to check if edge(i,j) is not passing vertically between two horizontally adjacent obstacles
int ispentrable_V(Coordinates *point,int i, int j,int *hurdles_position,int hp){
	int maxP,minP;
	if(point[i].x < point[j].x){
		maxP=j;
		minP=i;
	}
	else{
		maxP=i;
		minP=j;
	}
	int level = (minP/hp);
	if((minP-1)>=(level*hp)){
		while(minP<maxP){
			if(hurdles_position[minP]==1 && hurdles_position[minP-1]==1) return 0;
			minP+=hp;
		}
	}
	return 1;
}

//to check if edge(i,j) is not passing horizontally between two vertically adjacent obstacles
int ispentrable_H(Coordinates *point,int i, int j,int *hurdles_position,int hp){
	int maxP,minP;
	if(point[i].y < point[j].y){
		maxP=j;
		minP=i;
	}
	else{
		maxP=i;
		minP=j;
	}

	if((minP-hp)>=0){
		while(minP<maxP){
			if(hurdles_position[minP]==1 && hurdles_position[minP-hp]==1) return 0;
			minP+=1;
		}
	}
	return 1;
}



//used to convert string to integer(used for parsing command line input)
int convert_stoi(char *num){
	int n=strlen(num);
	int i,sum=0;
	for(i=0;i<n;i++){
		if((int)num[i]<48 || (int) num[i]>57) continue;
		int digit=(int)num[i];
		digit-=48;
		sum=sum*10+digit;
	}
	return sum;
}


//used in Dijkstra's algortihm for selecting the minimum cost vertex at every iteration 
//so that it can be used to relax other edges
int find_min_Vertex(double *d,int *v,int num_points){
	int minV=-1;
	for(int i=0;i<num_points;i++){
		if(v[i]==0 && (minV==-1 || d[i]<d[minV])) minV=i;
	}
	return minV;
}



int main(int argc, char *argv[]){
	int k,m,n,g;
	//cout<<"Please enter the value of k, m, n and g respectively:"<<endl;
	//cin>>k>>m>>n>>q;
	
	//command line inputs
	k=convert_stoi(argv[1]);
	m=convert_stoi(argv[2]);
	n=convert_stoi(argv[3]);
	g=convert_stoi(argv[4]);

	//checking if the provided input is invalid
	if(m==0 || n==0 || g==0 || k>(int)((m*n)/(g*g))){
		cout<<"INVALID INPUT!!"<<endl;
		return 0;
	}
	
	//start point on the xy-plane
	Coordinates topleft_point;
	topleft_point.x=20;
	topleft_point.y=30;

	//points generation
	int hor_points=(m/g)+1;
	int ver_points=(n/g)+1;
	int num_points=hor_points*ver_points;
	Coordinates point[hor_points*ver_points];
	for(int i=0;i<ver_points;i++){
		for(int j=0;j<hor_points;j++){
			point[i*hor_points+j].x = topleft_point.x + i*g;
			point[i*hor_points+j].y = topleft_point.y + j*g;
		}
	}

	//obstacles here are being reffered to as hurdles
	//hurdle points denotes the ****top_left corner**** of an obstacle(hurdle)
	//hurdles initializations
	int hurdles_position[hor_points*ver_points];
	for(int i=0;i<ver_points;i++){
		for(int j=0;j<hor_points;j++){
			hurdles_position[i*hor_points+j]=0;
		}
		
	}
	
	//random hurldes generation
	//randomly generated hurdles 
	int num_hurdles=k;
	while(num_hurdles--){
		int col_indx=rand()%(hor_points-1);
		int row_indx=rand()%(ver_points-1);
		if(hurdles_position[row_indx*hor_points+col_indx]==1){
			num_hurdles++;
		}
		else{
			hurdles_position[row_indx*hor_points+col_indx]=1;
		}
	}
	
	//just to view the output of points and hurdles generated(in terminal)****************************
	num_hurdles=0;
	cout<<"The points are given below with hurdles information:"<<endl;
	for(int i=0;i<ver_points;i++){
		for(int j=0;j<hor_points;j++){
			cout<<"("<<point[i*hor_points+j].x <<", "<<point[i*hor_points+j].y<<")-"<<hurdles_position[i*hor_points+j]<<"\t";
			if(hurdles_position[i*hor_points+j]==1) num_hurdles++;
		}
		cout<<endl;
		cout<<endl;
	}

	cout<<"\nTotal hurdles: "<<num_hurdles<<endl;
	cout<<"Total Points: "<<num_points<<endl;
	cout<<endl;
	//********************************************************************************************


	//generating the weighted adjacency matrix using the above points and hurdles informations
	double weight_matrix[num_points][num_points];
	for(int i=0;i<num_points;i++){
		for(int j=i;j<num_points;j++){
			if(i!=j && (point[i].x==point[j].x || point[i].y==point[j].y)){
				if(point[i].x==point[j].x && ispentrable_H(point,i,j,hurdles_position,hor_points)){
					//checking if the horontal edge(i,j) is not passing between two vertically adjacent hurdles(i not equal to j)
					weight_matrix[i][j]=abs(point[i].y-point[j].y)/g;
					weight_matrix[j][i]=abs(point[i].y-point[j].y)/g;
				}
				else if(point[i].y==point[j].y && ispentrable_V(point,i,j,hurdles_position,hor_points)){
					//checking if the vertical edge(i,j) is not passing between two horizontally adjacent hurdles(i not equal to j)
					weight_matrix[i][j]=abs(point[i].x-point[j].x)/g;
					weight_matrix[j][i]=abs(point[i].x-point[j].x)/g;
				}
				else{
					weight_matrix[i][j]=0;
					weight_matrix[j][i]=0;
				}
			}
			else if(i!=j && isclear(point,i,j,hurdles_position,num_points,g)){
				//checking if a diagonally slant edge(i,j) is not passing through any obstacle(i not equal to j)
				weight_matrix[i][j]=distance(point[i].x,point[i].y,point[j].x,point[j].y,g);
				weight_matrix[j][i]=distance(point[i].x,point[i].y,point[j].x,point[j].y,g);
			}
			else{ //otherwise
				weight_matrix[i][j]=0;
				weight_matrix[j][i]=0;
			}
		}
	}
	
	//Dijkstra's Algorihtm Implementation to find 
	//the shortest path from s(src: bottom-left vertex) to t(dest: top-right vertex) 
	//and algo its path length
	int s=hor_points*(ver_points-1),t=hor_points-1;
	double d[num_points];
	int v[num_points],parent[num_points];
	for(int i=0;i<num_points;i++){
		d[i]=INT_MAX;
		v[i]=0;
	}
	parent[s]=-1;
	d[s]=0;

	for(int i=0;i<num_points-1;i++){
		int minV=find_min_Vertex(d,v,num_points); // to select the minimum cost vertex for relaxing other vertices
		if(minV==t) break;
		v[minV]=1;
		for(int j=0;j<num_points;j++){
			if(weight_matrix[minV][j]!=0 && v[j]==0){
				if(d[minV]+weight_matrix[minV][j]<d[j]){
					d[j]=d[minV]+weight_matrix[minV][j];
					parent[j]=minV;
				}
			}
		}

	}

	//extracting ordered path from s(src: bottom-left vertex) to t(dest: top-right vertex) 
	int current_vertex=t,path[num_points];
	for(int i=0;i<num_points;i++) path[i]=-1;
	path[num_points-1]=t;
	int i=1;
	while(parent[current_vertex]!=-1 && i<num_points){
		path[num_points-1-i]=parent[current_vertex];
		current_vertex=parent[current_vertex];
		i++;
	}

	//just to view the minimum path length and the path (in terminal)
	//from s(src: bottom-left vertex) to t(dest: top-right vertex)
	cout<<"ST distance = "<<d[t]<<endl;
	cout<<"Path from s(src: bottom-left vertex) to t(dest: top-right vertex): ";
	for(int i=0;i<num_points;i++){
		if(path[i]!=-1) cout<<path[i]<<" ";
	}
	cout<<endl;

	//preprocessing the stepwise path length used for displaying in SVG 
	string dist_str;
	for(int i=0;i<num_points-1;i++){
  		if(path[i]!=-1){
  			double d_temp=weight_matrix[path[i]][path[i+1]];
  			dist_str.append(to_string(d_temp));	
  			if(i!=num_points-2) dist_str.append("+");
  		} 
  	}
  	dist_str.append("=");
  	dist_str.append(to_string(d[t]));

	//writing the obtained Output to svg
	FILE *fp;
	fp = fopen("spath.svg", "w");
  	fprintf(fp, "<?xml version=\"1.0\" standalone=\"no\" ?>\n");
  	fprintf(fp, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n\n");
  	fprintf(fp, "<svg width=\"1200px\" height=\"1200px\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");
  	fprintf(fp, "\n<text x=\"%d\" y=\"%d\" text-anchor=\"centre\" font-size=\"15px\" fill=\"#aaaaaa\">k=%d, m=%d, n=%d, g=%d</text>\n\n",(int)point[0].y*3,(int)point[0].x*2,k,m,n,g);
  	for (int i=0; i<ver_points; i++){
    	for(int j=0; j<hor_points; j++){
     		fprintf(fp, "<circle cx=\"%d\" cy=\"%d\" r=\"2\" stroke=\"black\" stroke-width=\"1\"/>\n",(int)point[i*hor_points+j].y*3, (int)point[i*hor_points+j].x*3);
     		if(hurdles_position[i*hor_points+j]==1) fprintf(fp, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:black;fill-opacity:0.3;\" />\n",(int)point[i*hor_points+j].y*3, (int)point[i*hor_points+j].x*3,g*3,g*3);
   		}
  	}
  	for(int i=0;i<num_points-1;i++){
  		if(path[i]!=-1){
  			fprintf(fp,"<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:rgb(255,0,0);stroke-width:2\" />\n",(int)point[path[i]].y*3,(int)point[path[i]].x*3,(int)point[path[i+1]].y*3,(int)point[path[i+1]].x*3);	
  		} 
  	}
  	fprintf(fp, "\n<text x=\"%d\" y=\"%d\" text-anchor=\"centre\" font-size=\"15px\" fill=\"red\">L=%s</text>\n\n",(int)point[0].y*3,(int)point[hor_points*(ver_points-1)].x*3+20,dist_str.c_str());
  	fprintf(fp, "</svg>\n");

  	cout<<"SVG File (\"spath.svg\") created."<<endl;
  	fclose(fp);




	return 0;
}