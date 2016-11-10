#include "InputZKOO.h"
#include "ZKOOMaker.h"

#include <map>

#include <algorithm>

namespace User
{
    using namespace cinder;

    struct InputZKOO::_m_InputZKOO
    {
        int frame;
        int successionFrame;
        bool isActive;

        std::map<uint32_t, ZKOOHand> recognition;
        std::map<uint32_t, ZKOOHand> push;
        std::map<uint32_t, ZKOOHand> press;
        std::map<uint32_t, ZKOOHand> pull;
        std::vector<uint32_t> handles;

        // １フレーム前のZKOOの状態を保存しておきます。
        std::vector<ZKOOHand> prevZKOOHands;
    };

    InputZKOO::InputZKOO( )
    {
        member = std::make_shared<InputZKOO::_m_InputZKOO>( );
        Resumption( );
    }
    bool InputZKOO::isRecognition( const uint32_t handle, ZKOOHand & touch ) const
    {
        auto itr = member->recognition.find( handle );
        if ( itr != member->recognition.cend( ) )
        {
            touch = itr->second;
            return true;
        }
        return false;
    }
    bool InputZKOO::isPush( const uint32_t handle, ZKOOHand& touch ) const
    {
        auto itr = member->push.find( handle );
        if ( itr != member->push.cend( ) )
        {
            touch = itr->second;
            return true;
        }
        return false;
    }
    bool InputZKOO::isPress( const uint32_t handle, ZKOOHand& touch ) const
    {
        auto itr = member->press.find( handle );
        if ( itr != member->press.cend( ) )
        {
            touch = itr->second;
            return true;
        }
        return false;
    }
    bool InputZKOO::isPull( const uint32_t handle, ZKOOHand& touch ) const
    {
        auto itr = member->pull.find( handle );
        if ( itr != member->pull.cend( ) )
        {
            touch = itr->second;
            return true;
        }
        return false;
    }
    void InputZKOO::end( ) const
    {
        flushInput( );
        member->handles.clear( );
    }
    void InputZKOO::flushInput( ) const
    {
        member->recognition.clear( );
        member->push.clear( );
        member->pull.clear( );
    }
    const std::vector<uint32_t>& InputZKOO::GetHandleIDs( ) const
    {
        return member->handles;
    }
    void InputZKOO::update( )
    {
        ZKOOUpdate( );
        ZKOOUsingRecognition( );
    }
    void InputZKOO::ZKOOUpdate( )
    {
        if ( !ZKOOMaker::Geti( ).IsHandUsing( ) ) return;

        for ( int i = 0; i < 2; ++i )
        {
            if ( IsNowRecognitionInput( i ) == true )
            {
                recognition( i, ZKOOMaker::Geti( ).hands[i] );
                // none
            }
            if ( IsPrevActiveInput( i ) == false && IsNowActiveInput( i ) == true )
            {
                push( i, ZKOOMaker::Geti( ).hands[i] );
                // push
            }
            if ( IsPrevActiveInput( i ) == true && IsNowActiveInput( i ) == true )
            {
                press( i, ZKOOMaker::Geti( ).hands[i] );
                // press
            }
            if ( IsPrevActiveInput( i ) == true && IsNowActiveInput( i ) == false )
            {
                pull( i, ZKOOMaker::Geti( ).hands[i] );
                // pull
            }
        }

        // ZKOOの状態を保存します。
        // このクラスは１フレーム前の状態と現在のフレームのZKOO状態を比べて判断しているので。
        member->prevZKOOHands = ZKOOMaker::Geti( ).hands;
    }

    void InputZKOO::ZKOOUsingRecognition( )
    {
        // 全体でのフレーム進行を保存します。
        member->frame = std::max( member->frame - 1, 0 );

        // ZKOOが認識されない時
        // !ZKOOMaker::Geti( ).IsHandUsing( )
        if ( !ZKOOMaker::Geti( ).IsHandUsing( ) )
        {
            member->successionFrame = std::max( member->successionFrame - 1, 0 );
        }
        // ZKOOは認識している
        else
        {
            // 片方の手が認識されない時
            // IsNowRecognitionInput( i ) == false
            // 左手と右手を確認する。
            // 片方でも認識できなかった場合も、カウントをする。
            for ( int i = 0; i < 2; ++i )
            {
                if ( IsNowRecognitionInput( i ) == false )
                {
                    member->successionFrame = std::max( member->successionFrame - 1, 0 );
                    break;
                }
            }
        }

        // フレームが0になったら連続的に認識できたかの確認をします。
        if ( member->frame == 0 )
        {
            // 60 * 3 フレームの間に、5フレームの誤認識があったら、ZKOOのアップデートを止めます。
            if ( member->successionFrame < 5 )
            {
                RecognitionInitialization( );
                member->isActive = false;
                // ZKOOのデータを初期化します。
                end( );
            }
            else
            {
                Resumption( );
            }
        }
    }

    void InputZKOO::RecognitionInitialization( )
    {
        member->frame = 60 * 3;
        member->successionFrame = 60 * 3;
    }

    void InputZKOO::Resumption( )
    {
        RecognitionInitialization( );
        member->isActive = true;
    }

    bool InputZKOO::IsActive( )
    {
        return member->isActive;
    }

    bool InputZKOO::IsHandsActive( )
    {
        return IsNowRecognitionInput( 0 ) && IsNowRecognitionInput( 1 );
    }

    bool InputZKOO::IsNowActiveInput( const uint32_t handle )
    {
        if ( ZKOOMaker::Geti( ).hands.size( ) == 2 )
        {
            return ZKOOMaker::Geti( ).hands[handle].IsTouching( );
        }
        return false;
    }

    bool InputZKOO::IsPrevActiveInput( const uint32_t handle )
    {
        if ( member->prevZKOOHands.size( ) == 2 )
        {
            return member->prevZKOOHands[handle].IsTouching( );
        }
        return false;
    }

    bool InputZKOO::IsNowRecognitionInput( const uint32_t handle )
    {
        if ( ZKOOMaker::Geti( ).hands.size( ) == 2 )
        {
            return ZKOOMaker::Geti( ).hands[handle].IsTracking( );
        }
        return false;
    }

    void InputZKOO::recognition( uint32_t handle, ZKOOHand event )
    {
        member->recognition.insert( std::make_pair( handle, event ) );

        member->handles.push_back( handle );
    }
    void InputZKOO::push( uint32_t handle, ZKOOHand event )
    {
        member->push.insert( std::make_pair( handle, event ) );
        member->press.insert( std::make_pair( handle, event ) );

        member->handles.push_back( handle );
    }
    void InputZKOO::press( uint32_t handle, ZKOOHand event )
    {
        member->press[handle] = event;

        member->handles.push_back( handle );
    }
    void InputZKOO::pull( uint32_t handle, ZKOOHand event )
    {
        member->pull.insert( std::make_pair( handle, event ) );
        member->press.erase( handle );

        member->handles.push_back( handle );
    }
}