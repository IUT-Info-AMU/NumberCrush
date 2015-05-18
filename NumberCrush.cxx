/**
 * \file      NumberCrush.cxx
 * \author    Brice Maussang, Gaëtan Perrot, Peio Rigaux, Jérémy Topalian
 * \date      16 Decembre 2014
 * \brief     Jeu NumberCrush
 * \version 1.0
 */


#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

/** \namespace nsNumberCrush
 * 
 * \brief namespace grouping tools components game's gameplay
 * 
 */

namespace nsNumberCrush
{
	typedef vector <string>				CVString; /*!<   \brief a type representing a matrix of string */
	typedef vector <unsigned>			CVUInt;/*!<   \brief a type representing a vector of unsigned */
	typedef vector <char>				CVLine;/*!<   \brief a type representing a row of the matrix */
	typedef vector <CVLine>				CMat;/*!<  \brief a type representing the matrix  */
	typedef pair <unsigned, unsigned>	CPosition;/*!<   \brief a type representing a position in the matrix  */
	typedef vector <CPosition>			CVPair;/*!< \brief a type representing a matrix of position  */

	//Colors
	
	const string KReset	 =  "0"; /*!< \brief a type to return to the original color */
	const string KBlack  =  "30";/*!< \brief a type to set a string in black */  
	const string KRed	 =  "31";/*!< \brief a type to set a string in red */
	const string KGreen  =  "32";/*!< \brief a type to set a string in green */
	const string KYellow =  "33";/*!< \brief a type to set a string in yellow */
	const string KBlue	 = "34";/*!< \brief a type to set a string in blue */
	const string KMagenta = "35";/*!< \brief a type to set a string in magenta */
	const string KCyan   =  "36";/*!< \brief a type to set a string in cyan */

	//Files Name
	const string KConfigFileName 	=	("config.cfg");/*!< \brief a type to name the config file */
	const string KHeaderFileName	=	("header.txt");/*!<   \brief  a type to name the header file */
	const string KMenuFileName		=	("menu.txt");/*!<  \brief a type to name the menu file */
	const string KRulesFileName		=	("rules.txt");/*!<  \brief  a type to name the rules file */
	const string KHelpFileName		=	("instructions.txt");/*!<  \brief  a type to name the help file */
	const string KCreditsFileName	=	("credit.txt");/*!<  \brief  a type to name the credit file */
	

    const unsigned KSquareValue = 10; /*!<  \brief a type representing the score of a cell */
	const char KImpossible		= '0';/*!<  \brief a type representing an empty cell */

	/**
	* \fn ClearScreen ()
	* \brief function to clear the display.
	*/
	void ClearScreen ()	//clear the screen
	{
		cout << "\033[H\033[2J";

	}// ClearScreen ()

	/**
	* \fn Color (const string & Col)
	* \brief function to change the color of the display.
	*
	*\param Col : Colors 
	*/
	void Color (const string& Col)	//add color to text
	{
		cout << "\033[" << Col << "m";

	}// Color ()

	/**
	* \fn ClearBuffer()
	* \brief function to clear only the buffer.
	*/
	void ClearBuffer ()	//clear the buffer
	{
		cin.clear ();
		cin.seekg (0, ios::end);

		if (!cin.fail ())
			cin.ignore (numeric_limits <streamsize>::max ());
		else
			cin.clear ();

	}// ClearBuffer ()

	/**
	* \fn Pause()
	* \brief function to set the program on pause.
	*/
	void Pause ()	//set the program on pause
	{
		cout << "Appuyez sur \"ENTRER\" pour continuer";
		cin.ignore ();
		cin.ignore ();

	}// Pause ()


	template <typename T>
	/**
	* \fn bool : IsOfType(const string& Str)
	* \brief function to determine the type.
	*\param Str : a string 
	*/
	bool IsOfType (const string& Str)
	{
		istringstream iss (Str);
		T tmp;

		return (iss >> tmp) && (iss.eof ());

	}// IsOfType ()


	template <typename T>
	/**
	* \fn bool : IsBetween(const T&  Val, const T Min, const T Max)
	* \brief function to determine a value between Min and Max.
	*\param Val : Value
	*\param Min : Minimum
	*\param Max : Maximum
	*/
	bool IsBetween (const T&  Val, const T Min, const T Max)
	{
		if ((Val <= Max) && (Val >= Min))
			return true;

		return false;

	}// IsBetween ()


	template <typename T>
	/**
	* \fn T ConvertStr (const string& Str)
	* \brief function to convert in a string type.
	*\param Str : a string 
	*/
	T ConvertStr (const string& Str)
	{
		istringstream iss (Str);
		T tmp;
		iss >> tmp;
		return tmp;

	}// ConvertStr ()

	/**
	* \fn bool : IsReadable (const string& FileName)
	* \brief function to determine if a file exist.
	*\param FileName : the name of the file 
	*/
	bool IsReadable (const string& FileName)	//test if a file exist
	{
		ifstream TestFile (FileName.c_str());
		return !TestFile.fail ();

	}// IsReadable ()

	/**
	* \fn DisplayFileContents (const string& FileName)
	* \brief function to show the file on screen.
	*\param FileName : the name of the file 
	*/
	void DisplayFileContents (const string& FileName)	//show a file on screen
	{
		if (IsReadable)
		{
			ifstream File (FileName.c_str ());
			string Line;
			while (getline (File, Line))
				cout << Line << endl;
		}
		else
			cerr << "Erreur d'ouverture du fichier '" << FileName << "'" << endl;

	}// DisplayFileContents ()

	/**
	* \fn DisplayScore (const unsigned& Score, const unsigned& TurnScore, const unsigned& Multiplier)
	* \brief function to show the score on screen.
	*\param Score : score of the player 
	*\param TurnScore : turn of the score 
	*\param Multiplier : score multiplier 
	*/
	void DisplayScore (const unsigned& Score,
					   const unsigned& TurnScore,
					   const unsigned& Multiplier)	//show score on screen
	{
		cout << "Score : " << Score << endl;
		cout << "+" << TurnScore 	<< endl;
		cout << "x" << Multiplier 	<< endl
									<< endl;

	}// ShowScore ()

	/**
	* \fn DisplayGrid (const CMat& Grid)
	* \brief function to show the matrix on screen.
	*\param Grid : name of the matrix
	*/
	void DisplayGrid (const CMat& Grid)	//show matrix on screen
	{
		Color (KReset);
		
		for (unsigned i (1); i < Grid [0].size () - 1; ++i)
			cout << "  " << i << " ";
		cout << endl;

		for (unsigned i (0); i < Grid [0].size () - 2; ++i)
			cout << "____";
		cout << endl;

		for (unsigned i (1); i < Grid.size () - 1; ++i)
		{
			for (unsigned j (0); j < Grid [i].size () - 2; ++j)
				cout << "|   ";
			cout << "|" << endl;

			cout << "| ";
			for (unsigned j (1); j < Grid [i].size () - 1; ++j)
			{
				(KImpossible != Grid [i][j]) ? cout << Grid [i][j]
											 : cout << " ";
				cout << " | ";
			}
			cout << setw (3) << i << endl;

			for (unsigned j (0); j < Grid [i].size () - 2; ++j)
				cout << "|___";
			cout << "|" << endl;
		}

	}// DisplayGrid ()

	/**
	* \fn InitGrid (CMat& Grid, const unsigned& Width, const unsigned& Heigth)
	* \brief function to create the square matrix.
	*
	*\param Grid : Matrix
	*\param Width : Width of the matrix
	*\param Heigth : Heigth of the matrix
	*/
	void InitGrid (CMat& Grid,
				   const unsigned& Width,
				   const unsigned& Heigth)	//Initialize a square matrix
	{
		Grid.resize (Heigth);
		for (unsigned i (0); i < Grid.size (); ++i)
			Grid [i].resize (Width);

		for (unsigned i (0); i < Grid.size (); ++i)
			for (unsigned j (0); j < Grid [i].size (); ++j)
				Grid [i][j] = KImpossible;

	}// InitGrid ()

	/**
	* \fn FillGrid (CMat& Grid, const CVLine& Vect)
	* \brief function to replace 'KImpossibe' values in a matrix by random characters from a vector.
	*
	*\param Grid : Matrix
	*\param Vect : Vector
	*/
	void FillGrid (CMat& Grid, const CVLine& Vect)	//replace 'KImpossibe' values in a matrix by random characters from a vector
	{
		for (unsigned i (1); i < Grid.size () - 1; ++i)
			for (unsigned j (1); j < Grid [i].size () - 1; ++j)
			{
				if (Grid [i][j] != KImpossible) continue;

				unsigned RandNb = rand () % Vect.size ();
				Grid [i][j] = Vect [RandNb];
			}

	}// FillGrid ()

	/**
	* \fn CatchInput (string& Input)
	* \brief function to catch a string.
	*
	*\param Input : Input of the player
	*/
	void CatchInput (string& Input)	//catch a string
	{
		for (getline (cin, Input); cin.eof (); getline (cin, Input))
			ClearBuffer ();

	}// CatchInput ()

	/**
	* \fn bool : IsValueInGrid (const CMat& Grid, const char& Value)
	* \brief function to test if a value is in a matrix.
	*\param Grid : Matrix
	*\param Value : Value of a cell
	*/
	bool IsValueInGrid (const CMat& Grid, const char& Value)	//test if a value is in a matrix
	{
		unsigned i (1);
		for ( ; i < Grid.size () - 1; )
		{
			unsigned j (1);
			for ( ; j < Grid [i].size () - 1; )
			{
				if (Grid [i][j] == Value) return true;
				++j;
			}
			++i;
		}

		return false;

	}// IsValueInVect ()

	/**
	* \fn string : TakeValueInVectStr (const string& Label, const CVString& Vect)
	* \brief function to catch a value in a vector of string.
	*\param Label : Value 
	*\param Vect : Vector of string
	*/
	string TakeValueInVectStr (const string& Label, const CVString& Vect)	//Catch a value in a vector of string
	{
		string Parser = "=";
		for(unsigned i (0); i < Vect.size (); ++i)
		{
			if(Label == Vect [i].substr (0, Vect [i].find (Parser)))
				return Vect [i].substr ((Vect [i].find (Parser) + Parser.length ()), Vect [i].length ());
		}

	}// TakeValueInVectStr ()

	/**
	* \fn FileToVectStr (CVString& Vect, const string& FileName)
	* \brief function to translate a file into a vector of string.
	*\param Vect : Vector of string
	*\param FileName : the name of the file 
	*/
	void FileToVectStr (CVString& Vect, const string& FileName)	//Translate a file into a vector of string
	{
		ifstream File (FileName.c_str ());
		if (File)
		{
			string Line;
			while (getline (File, Line))
				Vect.push_back (Line);
		}

	}// FileToVectStr ()

	/**
	* \fn CreateConfigFile ()
	* \brief function to create a config file with default values.
	*/
	void CreateConfigFile ()	//create config file with default values
	{
		ofstream ConfigCreated;
		ConfigCreated.open (KConfigFileName.c_str());
		ConfigCreated << "MatrixWidth=10" 	<< endl;
		ConfigCreated << "MatrixHeigth=10"	<< endl;
		ConfigCreated << "NbCandies=5" 		<< endl;
		ConfigCreated << "NbMaxTimes=20" 	<< endl;
		ConfigCreated.close ();

	}// CreateConfig ()

	/**
	* \fn ChangeValueInFile (const string& Label, const string& FileName, const string& Value)
	* \brief function to change the value in a file.
	*\param Label : Label
	*\param FileName : the name of the file 
	*\param Value : Value
	*/
	void ChangeValueInFile (const string& Label, const string& FileName, const string& Value)
	{
		string Parser = "=";
		CVString VToGetConfig;
		FileToVectStr (VToGetConfig, FileName);

		for (unsigned i (0); i < VToGetConfig.size (); ++i)
		{
			if (Label == VToGetConfig [i].substr (0, VToGetConfig [i].find (Parser)))
				VToGetConfig [i] = (VToGetConfig [i].substr (0, (VToGetConfig [i].find (Parser) + Parser.length ())) + Value);
		}

		ofstream Config (FileName.c_str (), ios_base::out | ios_base::trunc);
		string str;
		if (Config)
		{
			for (unsigned i (0); i < VToGetConfig.size (); ++i)
				Config << VToGetConfig [i] << endl;
		}
		Config.close();
	}

	/**
	* \fn CutInputStr (const string& InputStr, CPosition& Pos, char& C)
	* \brief function to cut the input in a position and a direction.
	*\param InputStr : input to cut
	*\param Pos : position 
	*\param C : direction
	*/
	void CutInputStr (const string& InputStr, CPosition& Pos, char& C)
	{
		istringstream iss (InputStr);

		string Str;
		iss >> Str;
		if (IsOfType <unsigned> (Str))
			Pos.first = ConvertStr <unsigned> (Str);
		iss >> Str;
		if (IsOfType <unsigned> (Str))
			Pos.second = ConvertStr <unsigned> (Str);
		iss >> Str;
		C = char (toupper (ConvertStr <char> (Str)));

	}// CutInputStr ()
					/**
					* \fn Test_CutInputStr ()
					* \brief test function @see CutInputStr.
					*/
					void Test_CutInputStr ()
					{
						string str = "11   12  z";
						CPosition pos;
						char c;

						CutInputStr (str, pos, c);
						cout << pos.first << "/" << pos.second << "/" << c << endl;

						str = "9     42   a";
						CutInputStr (str, pos, c);
						cout << pos.first << "/" << pos.second << "/" << c << endl;

					}// Test_CutInputStr ()

	/**
	* \fn  MakeAMove (CMat& Grid, const CPosition& Pos,	const char& Direction)
	* \brief function to move the current number according to the character in the 3rd parameter.
	*
	*\param Grid : Matrix 
	*\param Pos : the number's position before the move 
	*\param Direction : the key pressed by the player 
	*/
	void MakeAMove (CMat& Grid,
					const CPosition& Pos,
					const char& Direction)	//swap two values depending on a direction
	{
		switch (Direction)
		{
			case 'Z':
				swap (Grid [Pos.first][Pos.second],
					  Grid [Pos.first - 1][Pos.second]);
				break;
			case 'S':
				swap (Grid [Pos.first][Pos.second],
					  Grid [Pos.first + 1][Pos.second]);
				break;
			case 'Q':
				swap (Grid [Pos.first][Pos.second],
					  Grid [Pos.first][Pos.second - 1]);
				break;
			case 'D':
				swap (Grid [Pos.first][Pos.second],
					  Grid [Pos.first][Pos.second + 1]);
				break;
			default:
				cerr << "Direction Invalide" << endl;
		}

	}// MakeAMove ()

	/**
	* \fn bool AtLeastThreeInColumn (const CMat& Grid, CPosition& Pos, unsigned& HowMany)
	* \brief function to test if there is at least 3 consecutive numbers in the same column. 
	*
	*\param Grid : Matrix
	*\param Pos : the position from which we find the sequence 
	*\param HowMany : how many consecutive numbers we have from the position Position
	*/
	bool AtLeastThreeInColumn (const CMat& Grid,
							   CPosition& Pos,
							   unsigned& HowMany)	//find a sequence of at least three same numbers in a matrix column
	{
		for (unsigned i = Pos.first; i < Grid.size () - 2; ++i)
			for (unsigned j = Pos.second; j < Grid [i].size () - 1; ++j)
			{
				if (KImpossible == Grid [i][j]) continue;

				HowMany = 1;
				for ( ; Grid [i + HowMany][j] == Grid [i][j]; )
					++HowMany;

				if (3 <= HowMany)
				{
					Pos.first = i;
					Pos.second = j;
					return true;
				}
			}

		return false;

	}// AtLeastThreeInColumn ()

	/**
	* \fn bool AtLeastThreeInARow (const CMat& Grid, CPosition& Pos, unsigned& HowMany)
	* \brief function to test if there is at least 3 consecutive numbers in the same row. 
	*
	*\param Grid : Matrix
	*\param Pos : the position from which we find the sequence 
	*\param HowMany : how many consecutive numbers we have from the position Position
	*/
	bool AtLeastThreeInARow (const CMat& Grid,
							 CPosition & Pos,
							 unsigned& HowMany)	//find a sequence of at least three same numbers in a matrix line
	{
		for (unsigned i = Pos.first; i < Grid.size () - 1; ++i)
			for (unsigned j = Pos.second; j < Grid [i].size () - 2; ++j)
			{
				if (KImpossible == Grid [i][j]) continue;

				HowMany = 1;
				for ( ; Grid [i][j + HowMany] == Grid [i][j]; )
					++HowMany;

				if (3 <= HowMany)
				{
					Pos.first = i;
					Pos.second = j;
					return true;
				}
			}

		return false;

	}// AtLeastThreeInARow ()

	/**
	* \fn RemovalInColumn (CMat& Grid, const CPosition& Pos, const unsigned& HowMany)
	* \brief function to remove a sequence of numbers in a matrix column. 
	*
	*\param Grid : Matrix
	*\param Pos : the position from which we find the sequence 
	*\param HowMany : how many consecutive numbers we have from the position Position
	*/
	void RemovalInColumn (CMat& Grid,
						  const CPosition& Pos,
						  const unsigned& HowMany)	//remove a sequence of numbers in a matrix column
	{
		for (unsigned i (0); i < HowMany; ++i)
			Grid [Pos.first + i][Pos.second] = KImpossible;

	}// RemovalInColumn ()

	/**
	* \fn RemovalInRow (CMat& Grid, const CPosition& Pos, const unsigned& HowMany)
	* \brief function to remove a sequence of numbers in a matrix line. 
	*
	*\param Grid : Matrix
	*\param Pos : the position from which we find the sequence 
	*\param HowMany : how many consecutive numbers we have from the position Position
	*/
	void RemovalInRow (CMat& Grid,
					   const CPosition& Pos,
					   const unsigned& HowMany)	//remove a sequence of numbers in a matrix line
	{
		for (unsigned i (0); i < HowMany; ++i)
			Grid [Pos.first][Pos.second + i] = KImpossible;

	}// RemovalInRow ()

	/**
	* \fn  MoveNumbersDown (CMat& Grid)
	* \brief function to move all the matrix down. 
	*
	*\param Grid : Matrix
	*/
	void MoveNumbersDown (CMat& Grid)	//move all the matrix caracters down
	{
		for (unsigned i = Grid.size () - 2; i > 0; --i)
			for (unsigned j = Grid.size () - 2; j > 0; --j)
			{
				if (Grid [j][i] != KImpossible) continue;

				unsigned k = j - 1;
				for ( ;0 < k && Grid [k][i] == KImpossible; )
					--k;

				swap (Grid [k][i], Grid [j][i]);
			}

	}// MoveNumbersDown ()


	/**
	* \fn HandleGrid (CMat& Grid, CVPair& VPosColumn, CVPair& VPosRow, CVUInt& VHowManyCol,  CVUInt& VHowManyRow)
	* \brief function to find then remove a sequence of numbers in a matrix line. 
	*
	*\param Grid : Matrix
	*\param VPosColumn : the position from which we find the sequence in column
	*\param VPosRow : the position from which we find the sequence in row
	*\param VHowManyCol : how many consecutive numbers we have from the position VPosColumn
	*\param VHowManyRow : how many consecutive numbers we have from the position VPosRow
	*/
	void HandleGrid (CMat& Grid,
					 CVPair& VPosColumn,
					 CVPair& VPosRow,
					 CVUInt& VHowManyCol,
					 CVUInt& VHowManyRow)
	{
		unsigned HowMany;
		CPosition Pos;

		//find all sequences of at least three numbers in column
		for ( ; AtLeastThreeInColumn (Grid, Pos, HowMany); )
		{
			VPosColumn.push_back (Pos);
			VHowManyCol.push_back (HowMany);
			++Pos.second;
		}

		//find all sequences of at least three numbers in a row
		Pos.first = Pos.second = 0;
		for ( ; AtLeastThreeInARow (Grid, Pos, HowMany); )
		{
			VPosRow.push_back (Pos);
			VHowManyRow.push_back (HowMany);
			++Pos.second;
		}
		
		//remove sequences
		for (unsigned i (0); i < VPosColumn.size (); ++i)
			RemovalInColumn (Grid, VPosColumn [i], VHowManyCol [i]);
		for (unsigned i (0); i < VPosRow.size (); ++i)
			RemovalInRow (Grid, VPosRow [i], VHowManyRow [i]);

	}// HandleGrid ()

	/**
	* \fn   PlayScoreMod ()
	* \brief function to play score mod.
	*
	*/
	void PlayScoreMod ()	//play score mod
	{
		//init variables with config file
		CVString VConfig;
		FileToVectStr (VConfig, KConfigFileName);

		const unsigned MatHeigth	= ConvertStr <unsigned> (TakeValueInVectStr ("MatrixHeigth", VConfig)) + 2;
		const unsigned MatWidth		= ConvertStr <unsigned> (TakeValueInVectStr ("MatrixWidth", VConfig)) + 2;
		const unsigned NbCandies 	= ConvertStr <unsigned> (TakeValueInVectStr ("NbCandies", VConfig));
		unsigned NbMaxTimes 		= ConvertStr <unsigned> (TakeValueInVectStr ("NbMaxTimes", VConfig));

		unsigned TotalScore (0);
		unsigned TurnScore  (0);
		unsigned Multiplier (0);

		//init VCandies
		CVLine VCandies (NbCandies);
		for (unsigned i = (0); i < VCandies.size (); ++i)
			VCandies [i] = char ('0' + i + 1);

		//init board
		CMat Matrix;
		InitGrid (Matrix, MatWidth, MatHeigth);
		
		for ( ; IsValueInGrid (Matrix, KImpossible); )
		{
			CVPair VPosCol, VPosRow;
			CVUInt VHowManyCol, VHowManyRow;

			FillGrid (Matrix, VCandies);
			HandleGrid (Matrix, VPosCol, VPosRow, VHowManyCol, VHowManyRow);
			MoveNumbersDown (Matrix);
		}

		//start game
		for ( ; 0 != NbMaxTimes; )
		{
			//display board
			ClearScreen 	();
			DisplayScore 	(TotalScore, TurnScore, Multiplier);
			DisplayGrid 	(Matrix);
			cout << "Nombre de coups restant : " << NbMaxTimes << endl << endl;
			
			DisplayFileContents (KHelpFileName);
			cout << endl << "Entrez une commande : ";

			//input interpreter
			string Input;
			CMat M = Matrix;
			for ( ; M == Matrix; )
			{
				CPosition InputPos;
				char InputDir;

				CatchInput  (Input);
				CutInputStr (Input, InputPos, InputDir);

				if (IsBetween (InputPos.first, 1u, MatHeigth - 2) && IsBetween (InputPos.second, 1u, MatWidth - 2))
					MakeAMove (Matrix, InputPos, InputDir);
				else
					cout << "Commande invalide" << endl;
				if (IsValueInGrid (Matrix, KImpossible))
					MakeAMove (Matrix, InputPos, InputDir);
			}

			//manipulate matrix and increase score
			TurnScore = Multiplier = 0;
			unsigned Score;
			do
			{
				Score = TurnScore;
				CVPair VPosCol, VPosRow;
				CVUInt VHowManyCol, VHowManyRow;
				
				HandleGrid (Matrix, VPosCol, VPosRow, VHowManyCol, VHowManyRow);
				MoveNumbersDown (Matrix);
				FillGrid (Matrix, VCandies);

				for (unsigned i : VHowManyCol)
					TurnScore += i * KSquareValue;
				for (unsigned i : VHowManyRow)
					TurnScore += i * KSquareValue;
				Multiplier += VHowManyCol.size () + VHowManyRow.size ();
				
			}while (Score != TurnScore);

			TotalScore += TurnScore * Multiplier;

			--NbMaxTimes;
		}

		//show final score
		ClearScreen ();
		cout << "Votre score : " << TotalScore << endl;
		Pause ();

	}// PlayScoreMode ()

	/**
	* \fn   ChangeSettings ()
	* \brief function to change settings of the game.
	*
	*/
	void ChangeSettings ()
	{
		ClearScreen ();

		string Input;
		for ( ; Input != "Q"; )
		{
			CVString VConfig;
		FileToVectStr (VConfig, KConfigFileName);
		cout << "hauteur" << "  =  " << TakeValueInVectStr ("MatrixWidth", VConfig) << endl;
		cout << "largeur" << "  =  " << TakeValueInVectStr ("MatrixHeigth", VConfig) << endl;
		cout << "nombres" << "  =  " << TakeValueInVectStr ("NbCandies", VConfig) << endl << endl;

			cout << "Entrez le nom du paramètre suivi de la valeur que vous souhaitez lui attribuer." << endl
				 << "Tapez 'Q' pour quitter." << endl
				 << "ex: \"hauteur 15\"" << endl << endl;
			CatchInput (Input);
		
			istringstream iss (Input);
			string tmp, Value, Label;

			iss >> tmp;
			iss >> Value;
			unsigned Min, Max;

			if (tmp == "hauteur")
			{
				Label = "MatrixWidth";
				Min = 5;
				Max = 15;
			}
			else if (tmp == "largeur")
			{
				Label = "MatrixHeigth";
				Min = 5;
				Max = 20;
			}
			else if (tmp == "nombres")
			{
				Label = "NbCandies";
				Min = 3;
				Max = 7;
			}
			else
				cout << "Commande invalide" << endl;

			if (IsOfType <unsigned> (Value) && IsBetween (ConvertStr <unsigned> (Value), Min, Max))
				ChangeValueInFile (Label, KConfigFileName, Value);
			else
				cout << "Valeur invalide" << endl;
		}
		
		ClearScreen ();
		
	}

	/**
	* \fn   MainMenu ()
	* \brief function used to display the menu.
	*
	*/
	void MainMenu ()
	{
		string Str;
		for ( ;	Str != "4"; )
		{
			//display menu
			ClearScreen ();
			DisplayFileContents (KHeaderFileName);
			DisplayFileContents (KMenuFileName);

			CatchInput (Str);

			switch (ConvertStr <char> (Str))
			{
				case '1':
					PlayScoreMod ();	//play score mod
					break;
				case '2':
					DisplayFileContents (KRulesFileName);	//display rules
					Pause ();
					break;
				case '3':
					ChangeSettings ();	//change settings
					break;
				case '4':
					ClearScreen ();
					cout << "developped by" << endl;
					DisplayFileContents (KCreditsFileName);	//display credits
					Pause ();
					break;
				default :
					cerr << "Champ invalide" << endl;
			}
		}

	}// MainMenu ()

}// namespace

/**
 * \fn int main()
 * \brief Program main menu.
 *
 */
int main ()
{
	srand (time (NULL));

	if (!nsNumberCrush::IsReadable (nsNumberCrush::KConfigFileName))
		nsNumberCrush::CreateConfigFile ();
	
	nsNumberCrush::MainMenu ();
	
	return EXIT_SUCESS;

}// main ()

