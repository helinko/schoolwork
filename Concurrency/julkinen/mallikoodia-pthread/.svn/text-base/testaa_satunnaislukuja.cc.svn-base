
#include "satunnaisluku.hh"

#include <map>
#include <cstdint>
#include <iostream>


// 
// Arvotaan 10 miljoonaa lukua samalta väliltä ja
// lasketaan montako kertaa saamme kunkin luvun.
// Hyvässä jakaumassa kaikkia pitäisi esiintyä tasaisesti
//
void testaa_satunnaislukuja( void )
{
    Random r;
    std::map< uint32_t, unsigned int > laskuri;

    // arvonta
    for( int i = 0; i < 10000000 ; i ++ )
    {
        auto luku = r.integer( 30, 54 );
        if( laskuri.find( luku ) == laskuri.end() )
            laskuri[ luku ] = 1;
        else
            laskuri[ luku ]++;
    }

    // tulostus
    for( auto& item : laskuri ) 
    {
        std::cerr << item.first << " : " << item.second << std::endl;
    }
}

