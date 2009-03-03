#include "World.h"

#include "Entity.h"

World::~World()
{
	for ( EntityList::iterator e = _entities.begin( ); e != _entities.end( ); ++e )
	{
		SystemComponentList components = ( *e )->GetComponents( );

		for( SystemComponentList::iterator c = components.begin( ); c != components.end( ); ++c )
		{
			_systemScenes[ ( *c )->GetType( ) ]->DestroyComponent( ( *c ) );
		}
	}

	for ( SystemSceneList::iterator i = _systemScenes.begin( ); i != _systemScenes.end( ); ++i )
	{
		delete ( *i ).second;
	}
}

void World::RegisterSystem( ISystem* system )
{
	_systemScenes[ system->GetSystemType( ) ] = system->CreateScene( );
}

IEntity* World::CreateEntity( const std::string& name )
{
	IEntity* entity = new Entity( name );
	_entities.push_back( entity );
	return entity;
}