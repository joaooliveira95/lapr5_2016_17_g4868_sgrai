<h1><strong>SGRAI</strong></h1>

<p>&nbsp;</p>

<p>Fazer o download do template do projeto de SGRAI dispon&iacute;vel na p&aacute;gina de LAPR5.</p>

<p>Fazer download do OpenAL, do Glut e texture loader fornecido pelos professores.</p>

<p>Para correr o programa &eacute; necess&aacute;rio instalar o Glut, o OpenAL e o GLAUX. O tutorial de instala&ccedil;&atilde;o est&aacute; dispon&iacute;vel na p&aacute;gina de SGRAI no moodle, tal como as outras ferramentas.</p>

<p><strong>GLUT</strong></p>

<p>Link download GLUT: <a href="https://moodle.isep.ipp.pt/mod/url/view.php?id=54138">https://moodle.isep.ipp.pt/mod/url/view.php?id=54138</a></p>

<p>Configurar GLUT: <a href="https://moodle.isep.ipp.pt/mod/resource/view.php?id=54146">https://moodle.isep.ipp.pt/mod/resource/view.php?id=54146</a></p>

<p><strong>&nbsp;</strong></p>

<p><strong>&nbsp;</strong></p>
<h1><strong>SGRAI</strong></h1>
<p>&nbsp;</p>
<p>Fazer o download do template do projeto de SGRAI dispon&iacute;vel na p&aacute;gina de LAPR5.</p>
<p>Fazer download do OpenAL, do Glut e texture loader fornecido pelos professores.</p>
<p>Para correr o programa &eacute; necess&aacute;rio instalar o Glut, o OpenAL e o GLAUX. O tutorial de instala&ccedil;&atilde;o est&aacute; dispon&iacute;vel na p&aacute;gina de SGRAI no moodle, tal como as outras ferramentas.</p>
<p><strong>GLUT</strong></p>
<p>Link download GLUT: <a href="https://moodle.isep.ipp.pt/mod/url/view.php?id=54138">https://moodle.isep.ipp.pt/mod/url/view.php?id=54138</a></p>
<p>Configurar GLUT: <a href="https://moodle.isep.ipp.pt/mod/resource/view.php?id=54146">https://moodle.isep.ipp.pt/mod/resource/view.php?id=54146</a></p>
<p><strong>&nbsp;</strong></p>
<p><strong>&nbsp;</strong></p>
<p><strong>&nbsp;</strong></p>
<p><strong>&nbsp;</strong></p>
<p><strong>&nbsp;</strong></p>
<p><strong>GLAux</strong></p>
<p>Link download GLAux: <a href="https://moodle.isep.ipp.pt/mod/resource/view.php?id=54209">https://moodle.isep.ipp.pt/mod/resource/view.php?id=54209</a></p>
<p>Configurar GLAux: <a href="https://moodle.isep.ipp.pt/mod/resource/view.php?id=54208">https://moodle.isep.ipp.pt/mod/resource/view.php?id=54208</a></p>
<p><strong>&nbsp;</strong></p>
<p><strong>&nbsp;</strong></p>
<p><strong>&nbsp;</strong></p>
<p><strong>&nbsp;</strong></p>
<p><strong>OpenAL</strong></p>
<p>Download e configurara&ccedil;&atilde;o OpenAL: <a href="https://moodle.isep.ipp.pt/mod/page/view.php?id=54113">https://moodle.isep.ipp.pt/mod/page/view.php?id=54113</a></p>
<p>&nbsp;</p>
<p>1 - Descarregar e instalar o&nbsp;<a href="http://www.dei.isep.ipp.pt/~jpp/sgrai/openal/OpenAL11CoreSDK.exe">SDK do OpenAL</a>. Admitamos que ficou localizado em C:\Program Files\OpenAL 1.1 SDK<br /> <br /> 2 - Descarregar e instalar o&nbsp;<a href="http://www.dei.isep.ipp.pt/~jpp/sgrai/openal/freealut-1.1.0-bin.zip">ALUT</a>. Admitamos que ficou localizado em C:\Program Files\freealut-1.1.0<br /> <br /> 3 - Descarregar e executar o&nbsp;<a href="http://www.dei.isep.ipp.pt/~jpp/sgrai/openal/oalinst.zip">instalador do OpenAL</a>.<br /> <br /> 4 - Incluir no vosso programa o header file alut.h<br /> <br /> #include &lt;AL/alut.h&gt;<br /> <br /> 5 - Configurar o projecto na op&ccedil;&atilde;o:<br /> <br /> Project | Properties | Configuration Properties | C/C++ | General | Additional Include Directories<br /> <br /> com os seguintes caminhos:<br /> <br /> C:\Program Files\freealut-1.1.0\include<br /> C:\Program Files\OpenAL 1.1 SDK\include<br /> <br /> 6 - Configurar o projecto na op&ccedil;&atilde;o:<br /> <br /> Project | Properties | Configuration Properties | Linker | Input | Additional Dependencies<br /> <br /> com as seguintes bibliotecas:<br /> <br /> alut.lib OpenAL32.lib<br /> <br /> 7 - Configurar o vosso projecto na op&ccedil;&atilde;o:<br /> <br /> Project | Properties | Configuration Properties | Linker | General | Additional Library Directories<br /> <br /> com os seguintes caminhos:<br /> <br /> C:\Program Files\freealut-1.1.0\lib<br /> C:\Program Files\OpenAL 1.1 SDK\libs\Win32<br /> <br /> 8 - N&atilde;o esquecer de copiar o ficheiro alut.dll para C:\Windows\system32</p>
<p>&nbsp;</p>
<p>Outras configura&ccedil;&otilde;es:</p>
<p>Adicionar em Project | Properties | Configuration Properties | Linker | Input | Additional Dependencies: legacy_stdio_definitions.lib</p>
<p>Muda rem Project | Properties | Configuration Properties | Linker | Advanced | Image Has Safe Exception Handles :No (/SAFESEH:NO)</p>
<p>&nbsp;</p>
<p>Existe um ficheiro na p&aacute;gina de LAPR5 para dar apoio aos grafos do labirinto</p>
<p>O template tem um &uacute;nico project com o nome Lapr5.</p>
<p>Para o desenvolvimento da aplica&ccedil;&atilde;o &eacute; necess&aacute;rio:</p>
<p>Aplicar som;</p>
<p>Texturas;</p>
<p>Modela&ccedil;&atilde;o:</p>
<p>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Mdlviewer;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</p>
<p>Luz;</p>
<p>Implementa&ccedil;&atilde;o da personagem;</p>
<p>Dete&ccedil;&atilde;o de colis&otilde;es;</p>
<p>C&acirc;mara;</p>
<p>Overlays 2D com informa&ccedil;&otilde;es relativas ao POI e minimapa;</p>
<p>Utiliza&ccedil;&atilde;o do Rato;</p>
<p>Barra de estado com feedback informativo da rua, informa&ccedil;&atilde;o dos outros POI pr&oacute;ximos;</p>
<p>Informa&ccedil;&atilde;o do tempo percorrido e dist&acirc;ncia (barra de processo);</p>
<p>Implementa&ccedil;&atilde;o de Skybox;</p>
<p>Pode adicionar-se nevoeiro, chuva, etc;</p>
<p>Desenhar o grafo no ficheiro existente no projeto com o nome exemplo.grafo.</p>
<p>&nbsp;</p>
<p>Identifica&ccedil;&atilde;o de casos de Uso:</p>
<ul>
<li>Identificar Utilizador.</li>
<li>Obter POIs e percurso.</li>
<li>Gerar percurso e im&oacute;veis.</li>
</ul>






