#include <iostream>
#include <vector>
#include <set>
#include <queue>

enum class Color {
	WHITE, GRAY, BLACK
};

template<class T>
class Node {
	constexpr static Node<T>* NIL = nullptr;
	Node<T>* parent_;
	T id_;
	unsigned d_;
	unsigned f_;
	std::set<Node<T>*> neighbours_;
	Color color;

	template<class V> friend class Graph_list;
	template<class V> friend class Graph_matrix;

public:

	Node( T id ) :id_( id ) {}

	void add_neighbour( Node<T> *a ) {
		neighbours_.insert( a );
	}
};

template<class T>
class Graph_list {
	unsigned max_edge_;
	std::vector< Node<T>* > vertex_;

public:

	Graph_list( unsigned size ) :max_edge_( size ) {}

	void add_edge( T a, T b ) {

		if ( vertex_.size() <= max_edge_ ) {

			auto point1 = get( a );
			auto point2 = get( b );

			if ( point1 == Node<T>::NIL ) {
				vertex_.push_back( new Node<T>( a ) );
				point1 = vertex_.back();
			}
			if ( point2 == Node<T>::NIL ) {
				vertex_.push_back( new Node<T>( b ) );
				point2 = vertex_.back();
			}

			point1->add_neighbour( point2 );
			point2->add_neighbour( point1 );
		}
	}

	// searching Node with id `id` , if found returning that Node , else returning `NIL`
	Node<T>* get( T id ) {
		auto p = find_if( vertex_.begin(), vertex_.end(), [&]( Node<T>*x ) {return x->id_ == id; } );
		return  p == vertex_.end() ? Node<T>::NIL : *p;
	}

	void BFS( Node<T> *start ) {

		for ( Node<T>* node : vertex_ ) {
			node->color = Color::WHITE;
			node->d_ = 0;
			node->parent_ = Node<T>::NIL;
		}

		start->color = Color::GRAY;
		start->d_ = 0;
		start->parent_ = Node<T>::NIL;

		std::queue<Node<T>*> Q;
		Q.push( start );

		while ( !Q.empty() ) {
			Node<T>* u = Q.front();
			Q.pop();

			for ( Node<T>* v : u->neighbours_ ) {
				if ( v->color == Color::WHITE ) {
					v->color = Color::GRAY;
					v->d_ = u->d_ + 1;
					v->parent_ = u;
					Q.push( v );
				}
			}

			u->color = Color::BLACK;
		}
	}

	void DFS() {
		for ( Node<T> *u : vertex_ ) {
			u->color = Color::WHITE;
			u->parent_ = Node<T>::NIL;
		}
		time = 0;
		for ( Node<T> *u : vertex_ ) {
			if ( u->color == Color::WHITE ) {
				DFS_visit( u );
			}
		}
	}

	void print( Node<T>* start ) {
		for ( Node<T>* x : vertex_ ) {
			std::cout << x->id_ << " -> ";
			for ( Node<T>* y : x->neighbours_ ) {
				std::cout << y->id_ << ", ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		std::cout << "Distances from " << start->id_ << std::endl;
		for ( Node<T>* x : vertex_ ) {
			std::cout << x->id_ << "  ->  " << x->d_ << std::endl;
		}
		std::cout << std::endl;
	}

	~Graph_list() {
		for ( auto &x : vertex_ )
			delete x;
	}

private:
	unsigned time;
	void DFS_visit( Node<T>* u ) {
		time++;
		u->d_ = time;
		u->color = Color::GRAY;
		for ( Node<T>* v : u->neighbours_ ) {
			if ( v->color == Color::WHITE ) {
				v->parent_ = u;
				DFS_visit( v );
			}
		}
		u->color = Color::BLACK;
		time++;
		u->f_ = time;
	}
};

template<class T>
class Graph_matrix {
	// maximum number of edges
	static const unsigned MAXIMUM = 30;
	bool adjustency_matrix_[ MAXIMUM ][ MAXIMUM ];
	unsigned max_edge_;
	unsigned current_pos_ = 0;
	std::vector< std::pair< unsigned, Node<T>* > > vertex_;

public:
	Graph_matrix( unsigned size ) {
		max_edge_ = ( size < MAXIMUM ) ? size : MAXIMUM;
	}

	// searching Node with id `id` , if found returning that Node with it's index , else returning `NIL`
	std::pair<unsigned, Node<T>*> get( T id ) {
		auto it = std::find_if( vertex_.begin(), vertex_.end(), [&]( std::pair<unsigned, Node<T>*>& a ) {return ( a.second->id_ == id ); } );
		return  it == vertex_.end() ? std::pair<unsigned, Node<T>*>( {}, Node<T>::NIL ) : *it;
	}

	void add_edge( T a, T b ) {

		if ( current_pos_ <= max_edge_ ) {

			auto point1 = get( a );
			auto point2 = get( b );

			if ( point1.second == Node<T>::NIL ) {
				vertex_.push_back( { current_pos_, new Node<T>( a ) } );
				point1 = vertex_.back();

				for ( auto &i : vertex_ ) {
					adjustency_matrix_[ current_pos_ ][ i.first ] = false;
					adjustency_matrix_[ i.first ][ current_pos_ ] = false;
				}

				++current_pos_;
			}
			if ( point2.second == Node<T>::NIL ) {
				vertex_.push_back( { current_pos_, new Node<T>( b ) } );
				point2 = vertex_.back();

				for ( auto &i : vertex_ ) {
					adjustency_matrix_[ current_pos_ ][ i.first ] = false;
					adjustency_matrix_[ i.first ][ current_pos_ ] = false;
				}

				++current_pos_;
			}

			point1.second->add_neighbour( point2.second );
			point2.second->add_neighbour( point1.second );

			adjustency_matrix_[ point1.first ][ point2.first ] = true;
			adjustency_matrix_[ point2.first ][ point1.first ] = true;
		}
	}

	void BFS( Node<T> *start ) {

		for ( auto &node : vertex_ ) {
			node.second->color = Color::WHITE;
			node.second->d_ = 0;
			node.second->parent_ = Node<T>::NIL;
		}

		start->color = Color::GRAY;
		start->d_ = 0;
		start->parent_ = Node<T>::NIL;

		std::queue<Node<T>*> Q;
		Q.push( start );

		while ( !Q.empty() ) {
			Node<T>* u = Q.front();
			Q.pop();

			for ( Node<T>* v : u->neighbours_ ) {
				if ( v->color == Color::WHITE ) {
					v->color = Color::GRAY;
					v->d_ = u->d_ + 1;
					v->parent_ = u;
					Q.push( v );
				}
			}

			u->color = Color::BLACK;
		}
	}

	void DFS() {
		for ( auto &u : vertex_ ) {
			u.second->color = Color::WHITE;
			u.second->parent_ = Node<T>::NIL;
		}
		time = 0;
		for ( auto &u : vertex_ ) {
			if ( u.second->color == Color::WHITE ) {
				DFS_visit( u.second );
			}
		}
	}

	void print( Node<T>* start ) {

		std::cout << '\t';
		for ( auto &x : vertex_ ) {
			std::cout << x.second->id_ << '\t';
		}
		std::cout << std::endl << std::endl;

		for ( auto &x : vertex_ ) {
			std::cout << x.second->id_ << '\t';

			for ( unsigned i = 0; i < current_pos_; i++ ) {
				std::cout << adjustency_matrix_[ x.first ][ i ] << '\t';
			}

			std::cout << std::endl;
		}

		std::cout << std::endl;
		std::cout << "Distances from " << start->id_ << std::endl;
		for ( auto x : vertex_ ) {
			std::cout << x.second->id_ << "  ->  " << x.second->d_ << std::endl;
		}
		std::cout << std::endl;
	}

	~Graph_matrix() {
		for ( auto &x : vertex_ )
			delete x.second;
	}

private:
	unsigned time;
	void DFS_visit( Node<T>* u ) {
		time++;
		u->d_ = time;
		u->color = Color::GRAY;
		for ( Node<T>* v : u->neighbours_ ) {
			if ( v->color == Color::WHITE ) {
				v->parent_ = u;
				DFS_visit( v );
			}
		}
		u->color = Color::BLACK;
		time++;
		u->f_ = time;
	}
};

int main() {

#if 0
	Graph_matrix<std::string> gr( 5 );
	gr.add_edge( "Yerevan", "Tbilisi" );
	gr.add_edge( "Moscow", "Tbilisi" );
	gr.add_edge( "Yerevan", "Tehran" );
	gr.add_edge( "Berlin", "Moscow" );
	gr.add_edge( "Berlin", "Paris" );
	auto* source = gr.get( "Yerevan" ).second;

	gr.BFS( source );
	gr.print( source );
#endif


#if 0
	Graph_matrix<char> gr( 10 );
	gr.add_edge( 'v', 'r' );
	gr.add_edge( 'r', 's' );
	gr.add_edge( 's', 'w' );
	gr.add_edge( 'w', 't' );
	gr.add_edge( 'w', 'x' );
	gr.add_edge( 't', 'u' );
	gr.add_edge( 't', 'x' );
	gr.add_edge( 'x', 'u' );
	gr.add_edge( 'x', 'y' );
	gr.add_edge( 'u', 'y' );
	auto* source = gr.get( 's' ).second;

	gr.BFS( source );
	gr.print( source );
#endif

#if 0
	Graph_list<int> gr( 7 );
	gr.add_edge( 1, 2 );
	gr.add_edge( 1, 5 );
	gr.add_edge( 2, 5 );
	gr.add_edge( 2, 3 );
	gr.add_edge( 3, 4 );
	gr.add_edge( 4, 5 );
	gr.add_edge( 4, 6 );
	auto* source = gr.get( 1 );

	gr.BFS( source );
	gr.print( source );
#endif

#if 0
	Graph_matrix<int> gr( 7 );
	gr.add_edge( 1, 2 );
	gr.add_edge( 1, 5 );
	gr.add_edge( 2, 5 );
	gr.add_edge( 2, 3 );
	gr.add_edge( 3, 4 );
	gr.add_edge( 4, 5 );
	gr.add_edge( 4, 6 );
	auto* source = gr.get( 1 ).second;

	gr.DFS();
	gr.print( source );
#endif

	int var1 = 2, var2 = 4;
	var1 = var1 * var2;
	
}