# ComputerGraphics_HW1

이 프로그램은 것은 간단한 형태의 피카츄 배구 게임이다. 이 게임은 플레이어가 ‘피카츄’라는 캐릭터를 움직이며 네트 너머로 공을 넘겨 점수를 얻는 방식으로 진행된다. 공은 다른 Object나 화면 경계선에 닿으면 넘어가지 못하고 부딪히게 되며, 배구와 같이 공이 바닥에 닿게 되면 점수를 획득한다. 각각의 플레이어는 좌우 방향으로 움직일 수 있고, 마찬가지로 화면의 경계선이나 네트는 넘어가지 못한다.

게임을 실행하면 메뉴화면에서 Single Play 모드와 Multi Play 모드 둘 중에 하나를 고를 수 있다. Single Play 모드에서는 간단한 AI를 상대로 게임을 진행하게 되며 Multi Play 모드에서는 2명의 플레이어가 대전을 진행하게 된다.

소스 코드 파일은 GameObject를 위한 Object class의 구현 코드 (object.h, object.cpp, rectangleObject.h, rectanbleObject.cpp, movableRectangleObject.h, movableRectangleObject.cpp, ballObject.h, ballObject.cpp), rendering을 위한 함수 구현 코드 (render.h, render.cpp), game의 구체적인 구성요소와 logic을 구현한 코드(game.h, game.cpp), main 코드 (main.cpp)로 구성되어 있다.

메뉴화면에서는 위 아래 화살표 키로 Play 모드를 선택하고, 모드를 선택한 이후에 게임을 진행하게 된다. 싱글 플레이 모드에서는 a키와 d키를 이용해 좌우로 캐릭터를 움직이고 멀티 플레이 모드에서는 화살표 좌 우 키로 두번째 캐릭터를 움직일 수 있다. 또한, 게임 도중에 스페이스바 키를 누르면 normal camera 모드와 ball camera 모드를 전환할 수 있다.