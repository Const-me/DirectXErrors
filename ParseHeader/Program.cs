using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace ParseHeader
{
	static class Program
	{
		const string src = @"C:\Program Files (x86)\Windows Kits\10\Include\10.0.17134.0\um\Mferror.h";

		static void Main( string[] args )
		{
			try
			{
				using( var r = new StreamReader( src ) )
					parseMfErrors( r );
			}
			catch( Exception ex )
			{
				Console.WriteLine( ex.Message );
			}
		}

		static void parseMfErrors( TextReader input )
		{
			List<string> commentBlock = new List<string>();

			while( true )
			{
				string line = input.ReadLine();
				if( null == line )
					return;

				if( line.StartsWith( "//" ) )
				{
					line = line.Substring( 2 );
					if( String.IsNullOrWhiteSpace( line ) )
						continue;
					commentBlock.Add( line.Trim() );
					continue;
				}

				if( line.StartsWith( "#" ) )
				{
					if( line.StartsWith( "#define MF_E_" ) )
						parseCommentsBlock( line, commentBlock );
					commentBlock.Clear();
				}
			}
		}

		static void parseCommentsBlock( string define, List<string> commentBlock )
		{
			string[] fields = define.Split( " ".ToCharArray(), StringSplitOptions.RemoveEmptyEntries );
			string code = fields[ 1 ];
			string message = commentBlock.Last().Trim();
			if( message.EndsWith( "%0" ) )
				message = message.Substring( 0, message.Length - 2 );

			Console.WriteLine( "CHK_ERR( {0}, \"{1}\" )", code, message );
		}
	}
}