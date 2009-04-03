#include "BadArchive.h"

#include <fstream>

#include "FileManager.h"
#include "../System/Management.h"

#include "../Logging/Logger.h"
#include "../Exceptions/ArchiveNotFoundException.hpp"

Ogre::DataStreamPtr BadArchive::open( const Ogre::String& filename ) const
{
	DataStreamPtr stream;

	FileBuffer* fileBuffer = Management::GetInstance( )->GetFileManager( )->GetFile( filename, false );

	std::stringstream logMessage;
	logMessage << "BadArchive: Opening File " << filename;
	Logger::GetInstance( )->Debug( logMessage.str( ) );

	if ( fileBuffer != 0 )
	{
		MemoryDataStream memoryStream( fileBuffer->fileBytes, fileBuffer->fileLength, false );
		stream = DataStreamPtr( new MemoryDataStream( memoryStream, true ) );
		delete fileBuffer;
	}

	return stream;
}

bool BadArchive::exists( const Ogre::String& filename )
{
	return Management::GetInstance( )->GetFileManager( )->FileExists( filename );
}

StringVectorPtr BadArchive::find( const String& pattern, bool recursive /* = true */, bool dirs /* = false */ )
{
	std::stringstream mountPath;
	mountPath << "/data/" << mName << "/";

	StringVector* resultsVector = new StringVector( );

	FileSearchResultList* results = Management::GetInstance( )->GetFileManager( )->FileSearch( mountPath.str( ), pattern, true );

	for( FileSearchResultList::iterator i = results->begin( ); i != results->end( ); ++i )
	{
		FileSearchResult result = ( *i );

		_tableOfContents.insert( std::make_pair( result.FilePath, result ) );
		resultsVector->push_back( result.FilePath );
	}

	delete results;
	
	return StringVectorPtr( resultsVector );
}

FileInfoListPtr BadArchive::findFileInfo( const String& pattern, bool recursive /* = true */, bool dirs /* = false */ )
{
	FileInfoListPtr fileList = FileInfoListPtr( new FileInfoList( ) );

	int wildCardIndex = pattern.find( '*' );

	if ( wildCardIndex != pattern.npos )
	{
		std::string searchTerm = pattern.substr( wildCardIndex + 1, pattern.length( ) - wildCardIndex );

		for( TableOfContents::iterator i = _tableOfContents.begin( ); i != _tableOfContents.end( ); ++i )
		{
			if ( ( *i ).first.find( searchTerm ) != ( *i ).first.npos )
			{
				FileInfo fileInfo;

				fileInfo.archive = this;
				fileInfo.filename = ( *i ).second.FilePath;
				fileInfo.basename = ( *i ).second.FileName;

				fileList->push_back( fileInfo );
			}
		}
	}
	else
	{
		TableOfContents::iterator i = _tableOfContents.find( pattern );

		if ( i != _tableOfContents.end( ) )
		{
			FileInfo fileInfo;

			fileInfo.archive = this;
			fileInfo.filename = ( *i ).second.FilePath;
			fileInfo.basename = ( *i ).second.FileName;

			fileList->push_back( fileInfo );
		}
	}

	return fileList;
}

StringVectorPtr BadArchive::list( bool recursive /* = true */, bool dirs /* = false */ )
{
	return StringVectorPtr( new StringVector( ) );
}

FileInfoListPtr BadArchive::listFileInfo( bool recursive /* = true */, bool dirs /* = false */ )
{
	return FileInfoListPtr( new FileInfoList( ) );
}