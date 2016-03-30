Un reloxo sinxelo pero configurable.

Características:
        100% galego :-)  --- os nomes das variables na nosa lingua materna \\
        configurable a través dos axustes da esfera na app de pebble
        opcións para mostrar/ocultar día da semana, data do mes e carga da batería
        opción para invertir as cores do reloxo
        manexo seguro de memoria: todas filtracións están documentadas
        
            Ata o de agora inclúe 2 filtracións:
            
                app_message_open(38, 38);         --> 176B (necesario para a comunicación co móbil)
                tick_timer_service_subscribe();   -->  24B (con MINUTE_UNIT, para actualización do tempo)
            
            Estas 2 filtracións reportan un total de 200B non liberados ao sair da app. Isto non é problema
            sempre e cando o valor se manteña constante no tempo e a través dos cambios de estado da aplicación.
        
        
por facer:

        engadir créditos
        empregar clay.js (configuración fora de liña)
        engadir xeoposición e temperatura atmosférica (js + c)
        engadir opcións para o formato da data ("20 de marzo", "20/03", "20/03/2016") // int
        engadir opcións para o formato conxunto de día da semana e data ( "luns 20")  // bool
        engadir interfaces coa API de vibración, conexión bluetooth e acelerómetro para implementar:
        
            - vibración cada hora // bool
            - vibración cada (des)conexión a bluetooth // bool
            - cambio a 2ª pantalla, que mostra meteoroloxía extendida a través do movemento, durante uns segundos // int
