configure_file("Toolbar.txt" ${CMAKE_BINARY_DIR} COPYONLY)

set(RESOURCE_FILES
    coin.png
    enemy.png
    ground.png
    ladder.png
    player.png
    pole.png
    wall.png
    arial.ttf
    Toolbar.txt
    Board.txt
)

file(COPY ${RESOURCE_FILES}
     DESTINATION ${CMAKE_BINARY_DIR}/resources)
