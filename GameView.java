package gui.gameview;

import java.awt.Color;
import java.awt.Dimension;

///////////////////////////   [==  Star Quest  ==]   ///////////////////////////////
//  [IMPORTANT] Pour une meilleur lisibilité, masquer les pavés de commentaire avec l'IDE, ces commentaires sont surtout reservés à la documentation HTML (Javadoc).
//////////////////////////////////////////////////////////////////////////////////////////////////////////

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.IllegalComponentStateException;
import java.awt.Image;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import javax.swing.ImageIcon;
import javax.swing.JPanel;

import controller.Config;
import controller.GameUpdater;
import gui.Refreshable;
import gui.gameview.backpackpanel.BackPackPanel;
import gui.gameview.scope.ScopeIcon;
import gui.gameview.scope.ScopeManage;
import gui.gameview.scope.ScopePanel;
import gui.gameview.textpopup.TextPopUp;
import model.Player;
import model.map.Map;
import model.memRes.Ressources;

/**
 * Cette classe représente l'interface de jeu à part entière, elle va contenir le HUD (les boutons, la barre de vie, etc), 
 * ainsi que tous les éléments du jeu comme, le joueur, le ennemis,etc
 */
public class GameView extends JPanel implements Refreshable{

    /**
     * l'interface graphique qui va contenir toutes les objet de jeu jeu comme le joueurs, les énnemis, etc
     */
    private InGameView inGameView;

    /**
     * Cet attribut sera utilisé dans un Thread qui va repaindre en permanence le JPanel pour éviter les effets de glich à l'écran.
     */
    public boolean repaint = true;

    /**
     * Panneau donnant des informations sur les armes.
     */
    private ArmingInterface armingPanel = new ArmingInterface();

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les entités tels que les ennemis et le joueur.
     */
    private JPanel entityPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les items du jeu.
     */
    private JPanel itemPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les Murs du jeu.
     */
    private JPanel wallsPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les effets visuels du jeu.
     */
    private JPanel fxPanel;

    /**
     * Il s'agit du menu de l'inventaire du joueur.
     */
    private BackPackPanel backpackmenu;

    /**
     * Il s'agit de la référence à la carte actuel de la partie.
     */
    private Map map;

    /**
     * Il s'agit du panneau affichant la valeur actuelle du facteur d'echelle de la carte.
     */
    private ScopePanel scopePanel = new ScopePanel();
    
    private TextPopUp barrierEnergyPanel;

    ///**
    // * Fonctionalité abandonnée
    // */
    ///**
    // * Paneau affichant le menu des item à ramasser.
    // */
    // private LootMenu lootMenuPanel = new LootMenu();
    
    /**
     * Constructeur de GameView, il prend en argument les dimension de l'interface de jeu (InGameView)
     */
    public GameView(int decalageX, int decalageY, int width, int height, String bakcgroundLocation){
        Config.window.setContentPane(this); //GameView devient le paneau principale de l'interface.
        this.setLayout(null); //GameView n'a pas de Layout.
        this.inGameView = new InGameView(decalageX, decalageY, width, height, bakcgroundLocation); //Création d'une view du jeu, 
        this.add(inGameView); //Ajout dans l'interface de jeu dans l'interface global GameView.
        this.add(this.armingPanel,0);
        Config.elementRefreshables.add(this); //Ajout de l'interface de jeu comme étant un refreshable, dans Config.
        Config.updatable.add(this);
        this.backpackmenu = new BackPackPanel(null);
        this.setBounds(0,0,Config.screenW, Config.screenH);
        this.setPreferredSize(new Dimension(Config.screenW, Config.screenH));

        // new Thread(()->{
        //     while(repaint){
        //         repaint();
        //         try {
        //             Thread.sleep(1);
        //         } catch (InterruptedException e) {
        //         }
        //     }
        // },"GameView - Repainting").start();

        this.initilizeCanvas(width, height);
        /**
         * Fonctionalité abandonnée
         */
        // this.inGameView.add(lootMenuPanel); //Ajout du menu des loot.
        this.add(backpackmenu, 0);

        Integer [] coords = {
            Config.adaptValueToScreen(30, 1),
            Config.screenH - Config.adaptValueToScreen(100, -1)
        };
        this.barrierEnergyPanel = new TextPopUp("Energie : 0", Color.white, 10, Ressources.PATH+"null.png", coords);
        
    }

    public void setDecalageY(int y){
       inGameView.decalageY = y;
       inGameView.refreshGraphics();
    }

    /**
     * Cette fonction va initialiser tous les panneaux du jeu qui vonte servir de calques pour les différents éléments du jeu,  
     * tels que les entités les murs, les effets visuels, etc.
     * <p>Tous ces panneaux seront contenu dans {@link InGameView}, ainsi ils auront les mêmes dimensions que le inGameView.
     * 
     * @param dX Le décalage dans l'axe X du panneau.
     * @param dY Le décalage dans l'axe Y du panneau.
     * @param w La largeur des panneaux.
     * @param h La hauteur des panneaux.
     */
    private void initilizeCanvas(int w, int h){
        initWallsPanel(w, h);
        initItemPanel(w, h);
        initEntityPanel(w, h); //Création de du panneau des entités.
        initFxPanel(w, h);
        initScopePanel();
    }   
            /**
             * Fonction qui initialise le panneau affichant le facteur de zoom de la carte.
             */
            private void initScopePanel(){
                this.scopePanel = new ScopePanel();
                this.add(scopePanel, 0);
                this.add(scopePanel.scopeIcon.textScope, 0);
            }


            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             * 
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initEntityPanel(int w, int h){
                this.entityPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.entityPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.entityPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.entityPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.entityPanel,0); //Ajout du panneau dans l'interface de jeu.
            }
            
            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initItemPanel(int w, int h){
                this.itemPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.itemPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.itemPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.itemPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.itemPanel,0); //Ajout du panneau dans l'interface de jeu.
            }

            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initWallsPanel(int w, int h){
                this.wallsPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.wallsPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.wallsPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.wallsPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.wallsPanel,0); //Ajout du panneau dans l'interface de jeu.
            }

            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initFxPanel(int w, int h){
                this.fxPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.fxPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.fxPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.fxPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.fxPanel,0); //Ajout du panneau dans l'interface de jeu.
            }





    /**
     * C'est cette Objet de l'interface graphique qui va contenir tous les éléments du jeu comme le joueurs, les énnemis.
     */
    public class InGameView extends JPanel implements Refreshable{

        /**
         * Il s'agit ici d'un décalage, vers la gauche du panneau.
         * <p>
         * <strong>Explication :</strong> La carte du jeu sera faite de manière à avoir un peu plus d'éléments en dehors
         *  de l'écran (C'est à dire en gros que certains éléments seront en dehors de l'écran, exprès). Cela permettera 
         * de faire l'implémentation de fonctionnalités très intéressantes dans le jeu, comme par exemple, des effets de 
         * tremblement de l'écran (Shake), de plus en plus grand pour simuler des effet de caméra lors du tir.
         * 
         * Mais également cela permettra de rajouter des fonctionnalités comme les zooms avec des armes de précision, 
         * (sniper), dans lesquels la map deviendra de plus petite et affichera plus d'éléments, selon le type de zoom
         * 
         * Le décalage en X est alors le décalage vers la gauche qui ferra que le joueur verra ce qui est au centre, mais 
         * pas les alentours.
         */
        private int decalageX; 

        /**
         * Il s'agit ici d'un décalage, vers le bas du panneau.
         * <p>
         * <strong>Explication :</strong> La carte du jeu sera faite de manière à avoir un peu plus d'éléments en dehors
         *  de l'écran (C'est à dire en gros que certains éléments seront en dehors de l'écran, exprès). Cela permettera 
         * de faire l'implémentation de fonctionnalités très intéressantes dans le jeu, comme par exemple, des effets de 
         * tremblement de l'écran (Shake), de plus en plus grand pour simuler des effet de caméra lors du tir.
         * 
         * (Le mode sniper ne changera pas la vue de haut en bas mais uniquement de droite à gauche)
         * 
         * Le décalage en Y est alors le décalage vers le bas qui ferra que le joueur verra ce qui est au centre, mais 
         * pas les alentours.
         */
        private int decalageY;

        /**
         * Panneau qui contiendera l'image d'arrière plan du jeu.
         */
        private JPanel background;

        /**
         * Booléen utilisé pour faire fonctionner le Thread qui faitb bouger la carte.
         * <p>Tant qu'il vaut {@code true} le background du jeu sera animé.
         */
        private boolean backgroundThreadactive = true;

        /**
         * Eniter représentant le décalage à ajouter à l'arrière plan pour le décaler en haut ou en bas.
         */
        private int bacgroundDY = 1;

        /**
         * Constructeur de l'interface, il prend en argument les dimension de l'interface
         */
        InGameView(int decalageX, int decalageY, int width, int height, String backgroundLocation){
            this.decalageX = decalageX; //Mise à jour du décalage en X
            this.decalageY = decalageY; //Mise à jour du décalage en Y
            this.setBounds( //Mise à l'échelle de l'interface de jeu
                decalageX,
                decalageY,
                width,
                height
            );
            this.setLayout(null); //Pas de Layout
            this.setFocusable(true);
            this.setOpaque(false); //Activation de la transparence
            // this.setBackground(new Color(200, 111, 33));
            this.initBackground(backgroundLocation);
            this.add(background);
            this.makeThreadBackgroundMoving();
            this.addMouseWheelActions();
            // System.out.println("inGameView : [X = "+decalageX+", Y = "+decalageY+", Width = "+width+", Height = "+height); //debug
        }

                /**
                 * Fonction qui ajoute les actions à faire lorsque molette de la souris est enclenchée.
                 */
                private void addMouseWheelActions(){

                    this.addMouseWheelListener(new MouseWheelListener() {

                        @Override
                        public void mouseWheelMoved(MouseWheelEvent e){
                            //Délai pour éviter que l'intervalle d'exécution de la fonction ne soit trop rapide.
                                // if (System.currentTimeMillis() - delay < 200)
                                //     return;
                                // delay = System.currentTimeMillis();
                            /////////////////////////////////


                            int dir = e.getWheelRotation(); //Direction actuelle de la rotation de la molette.

                            if (dir < 0) { 
                                //"Molette tournée vers le haut"
                                // System.out.println("Haut");
                                if(GameView.this.map.getFov()>12) {//Si le FOV est au dessus de douze.
                                    GameView.this.map.resetFov(GameView.this.map.getFov()-1);
                                    updateScopePanel();
                                }
                                
                            } else {
                                //"Molette tournée vers le bas"
                                // System.out.println("bas");
                                if(GameView.this.map.getFov()<20) //Si le FOV est en dessous de vingt.
                                    GameView.this.map.resetFov(GameView.this.map.getFov()+1);
                                    updateScopePanel();
                            }

                        }
                    });

                }

                    private void updateScopePanel(){
                        
                        int fov = GameView.this.map.getFov();
                        int percentage = (fov-12) * 100 / 8;
                        System.out.println(percentage);
                        int scopeBarHeight = ((GameView)Config.window.getContentPane()).getScopePanel().scopeManage.getHeight();
                        int Ysep = scopeBarHeight * percentage / 100;
                        ScopeIcon sI = ((GameView)Config.window.getContentPane()).getScopePanel().scopeIcon;
                        ScopeManage sM = ((GameView)Config.window.getContentPane()).getScopePanel().scopeManage;
                        sI.setLocation(sI.getX(), Ysep + Config.adaptValueToScreen(80, -1) - sI.getHeight()/2);
                        sM.updateYSep(Ysep);
                    }


                private void initBackground(String backgroundLocation){
                    this.background = new JPanel(){

                        private Image background = new ImageIcon(backgroundLocation).getImage();

                        {

                            this.setBounds(
                                0,
                                Config.adaptValueToScreen(-10000, 1),
                                InGameView.this.getWidth(),
                                InGameView.this.getHeight() + Config.adaptValueToScreen(10000, -1)
                            );
                            this.setOpaque(false);

                        }

                        @Override
                        public void paintComponent(Graphics g){
                            Graphics2D g2d = (Graphics2D)g;
                            g2d.drawImage(background, 0, 0, this.getWidth(), this.getHeight(), null);
                            super.paintComponent(g);
                        }

                    };
                }
                
                /**
                 * Cette fonction lance un Thread qui va faire descendre le background vers le bas en permanence.
                 */
                private void makeThreadBackgroundMoving(){
                    Thread thread = 
                        new Thread(
                            ()->{
                                while(backgroundThreadactive && GameUpdater.isThreadWorking){ //Tant qu'il est possible d'animer l'arrière plan.
                                    try{
                                        updateBackgroundLocation();
                                        Thread.sleep(20);
                                    }catch(InterruptedException e){
                                        System.out.println("Thread d'animation d'arrière plan stoppé !");
                                    }
                                    catch(IllegalComponentStateException e){
                                        //Ce n'est pas grave, cela est due au fait que tous les éléments ne sont pas encore initilisés au début du Thread.
                                    }
                                }
                            },
                            "GameView - Background Moving Thread"
                        );
                        
                    Config.toInterupt.add(thread);
                    thread.start();
                }

                        /**
                         * Fonction qui mets à jour la position de l'arrière plan pour faire l'animation.
                         * @throws IllegalComponentStateException Lorsque tous les éléments ne sont pas encore initilisés au début du Thread.
                         */
                        private void updateBackgroundLocation() throws IllegalComponentStateException{
                            
                            if(GameView.this.getLocationOnScreen().y <= this.background.getLocationOnScreen().y){
                                this.background.setBounds(
                                    0,
                                    this.getY() + Config.adaptValueToScreen(-10000, -1),
                                    this.background.getWidth(),
                                    this.background.getHeight()
                                );
                            }
                            else{
                                this.background.setBounds(
                                    0,
                                    this. background.getY() + this.bacgroundDY,
                                    this.background.getWidth(),
                                    this.background.getHeight()
                                );
                            }

                        }

    
        /**
         * Fonction qui va rafraichir la dimension des éléments à l'écran, si jamais l'écran est redimensionné pendant la 
         * partie
         */
        @Override
        public void refreshGraphics(){
            this.setBounds( //Mise à l'échelle de l'interface de jeu
                Config.adaptValueToScreen(decalageX, 1), 
                Config.adaptValueToScreen(decalageY, -1),
                Config.adaptValueToScreen(this.getWidth(), 1),
                Config.adaptValueToScreen(this.getHeight(), -1)
            );
            this.setLayout(null); //Pas de Layout
        }
        
        /**
         * Fonction qui arrête l'animation de l'arrière plan du jeu.
         */
        public void stopBackgroundAnime(){
            this.backgroundThreadactive = false;
        }

        /**
         * Fonction qui revoie la valeur actuelle du décalage de l'arrière plan.
         * @return Décalage de l'arrière plan.
         */
        public int getBackgroundDY(){
            return this.bacgroundDY;
        }
        
        /**
         * Fonction qui change le décalage de l'arrière plan.
         * @param bacgroundDY Nouveau décalage de l'arrière plan
         */
        public void setBackgroundDY(int bacgroundDY){
            this. bacgroundDY= bacgroundDY;
        }
    }


    /**
     * Fonction qui va rafraichir la dimension des éléments à l'écran, si jamais l'écran est redimensionné pendant la 
     * partie
     */
    @Override
    public void refreshGraphics(){
        this.setBounds(0,0,Config.screenW, Config.screenH); //Remise à l'échelle du GameView.
        this.inGameView.refreshGraphics(); //Remise à l'echelle du JPanel contenant tous les éléments du jeu.
    }
  
    /**
     * Getter de l'interface de jeu
     */
    public InGameView getInGameView(){return this.inGameView;}

    public ArmingInterface getArmingInterface(){
        return this.armingPanel;
    }

    

    /**
     * Fonction qui renvoi le panneau des entités.
     * <p>Va servir lors de l'ajout des entités dans le jeu tels que le joueur ou les ennemis.
     * @return Le panneau des entités dans le jeu.
     */
    public JPanel getEntityPanel(){
        return this.entityPanel;
    }

    /**
     * Fonction qui renvoi le panneau des items du jeu.
     * <p>Va servir lors de l'ajout des items dans le jeu.
     * @return Le panneau des items dans le jeu.
     */
    public JPanel getItemPanel(){
        return this.itemPanel;
    }

    /**
     * Fonction qui renvoi le panneau des murs du jeu.
     * <p>Va servir lors de l'ajout des murs dans le jeu.
     * @return Le panneau des murs dans le jeu.
     */
    public JPanel getWallsPanel(){
        return this.wallsPanel;
    }

    /**
     * Fonction qui renvoi le panneau des effets visuels du jeu.
     * <p>Va servir lors de l'ajout des effets visuels dans  le jeu.
     * @return Le panneau des effets visuels du jeu.
     */
    public JPanel getFxPanel(){
        return this.fxPanel;
    }

    /**
     * Fonction qui renvoi le panneau de l'inventaire du joueur. 
     * @return Le panneau de l'inventaire.
     */
    public BackPackPanel getBackPackPanel(){
        return this.backpackmenu;
    }

    /**
     * Fonction qui renvoie la référence de la carte.
     * @return La référence de la carte.
     */
    public Map getMap(){return this.map;}

    /**
     * Fonction qui renvoie le panneau affichant le facteur d'echelle de la carte.
     * @return Le panneau affichant le facteur d'echelle de la carte.
     */
    public ScopePanel getScopePanel(){return this.scopePanel;}


    /**
     * Fonction qui change la réfénrence du joueur.
     * @param player la nouvelle référence du joueur.
     */
    public void setBackPackMenuPlayer(Player player){
        this.backpackmenu.setPlayer(player);
    }


    /**
     * Fonction qui met à jour la référence de la carte actuelle du jeu.
     * @param map La nouvelle carte.
     */
    public void setMap(Map map){
        this.map = map;
    }


    //Fonctionnalité abandonnée
    // /**
    //  * Fonction qui renvoi le menu des items.
    //  * @return Le menu des items.
    //  * @see LootMenu
    //  */
    // public LootMenu getLootMenuPanel() {
    //     return lootMenuPanel;
    // }
    
    //Fonctionnalité abandonnée
    // /**
    //  * 
    //  * Fonction qui change le menu des item.
    //  * @return Le nouveau menu des items.
    //  * @see LootMenu
    //  */
    // @Deprecated
    // public void setLootMenuPanel(LootMenu lootMenuPanel) {
    //     this.lootMenuPanel = lootMenuPanel;
    // }

}
package gui.gameview;

import java.awt.Color;
import java.awt.Dimension;

///////////////////////////   [==  Star Quest  ==]   ///////////////////////////////
//  [IMPORTANT] Pour une meilleur lisibilité, masquer les pavés de commentaire avec l'IDE, ces commentaires sont surtout reservés à la documentation HTML (Javadoc).
//////////////////////////////////////////////////////////////////////////////////////////////////////////

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.IllegalComponentStateException;
import java.awt.Image;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import javax.swing.ImageIcon;
import javax.swing.JPanel;

import controller.Config;
import controller.GameUpdater;
import gui.Refreshable;
import gui.gameview.backpackpanel.BackPackPanel;
import gui.gameview.scope.ScopeIcon;
import gui.gameview.scope.ScopeManage;
import gui.gameview.scope.ScopePanel;
import gui.gameview.textpopup.TextPopUp;
import model.Player;
import model.map.Map;
import model.memRes.Ressources;

/**
 * Cette classe représente l'interface de jeu à part entière, elle va contenir le HUD (les boutons, la barre de vie, etc), 
 * ainsi que tous les éléments du jeu comme, le joueur, le ennemis,etc
 */
public class GameView extends JPanel implements Refreshable{

    /**
     * l'interface graphique qui va contenir toutes les objet de jeu jeu comme le joueurs, les énnemis, etc
     */
    private InGameView inGameView;

    /**
     * Cet attribut sera utilisé dans un Thread qui va repaindre en permanence le JPanel pour éviter les effets de glich à l'écran.
     */
    public boolean repaint = true;

    /**
     * Panneau donnant des informations sur les armes.
     */
    private ArmingInterface armingPanel = new ArmingInterface();

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les entités tels que les ennemis et le joueur.
     */
    private JPanel entityPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les items du jeu.
     */
    private JPanel itemPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les Murs du jeu.
     */
    private JPanel wallsPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les effets visuels du jeu.
     */
    private JPanel fxPanel;

    /**
     * Il s'agit du menu de l'inventaire du joueur.
     */
    private BackPackPanel backpackmenu;

    /**
     * Il s'agit de la référence à la carte actuel de la partie.
     */
    private Map map;

    /**
     * Il s'agit du panneau affichant la valeur actuelle du facteur d'echelle de la carte.
     */
    private ScopePanel scopePanel = new ScopePanel();
    
    private TextPopUp barrierEnergyPanel;

    ///**
    // * Fonctionalité abandonnée
    // */
    ///**
    // * Paneau affichant le menu des item à ramasser.
    // */
    // private LootMenu lootMenuPanel = new LootMenu();
    
    /**
     * Constructeur de GameView, il prend en argument les dimension de l'interface de jeu (InGameView)
     */
    public GameView(int decalageX, int decalageY, int width, int height, String bakcgroundLocation){
        Config.window.setContentPane(this); //GameView devient le paneau principale de l'interface.
        this.setLayout(null); //GameView n'a pas de Layout.
        this.inGameView = new InGameView(decalageX, decalageY, width, height, bakcgroundLocation); //Création d'une view du jeu, 
        this.add(inGameView); //Ajout dans l'interface de jeu dans l'interface global GameView.
        this.add(this.armingPanel,0);
        Config.elementRefreshables.add(this); //Ajout de l'interface de jeu comme étant un refreshable, dans Config.
        Config.updatable.add(this);
        this.backpackmenu = new BackPackPanel(null);
        this.setBounds(0,0,Config.screenW, Config.screenH);
        this.setPreferredSize(new Dimension(Config.screenW, Config.screenH));

        // new Thread(()->{
        //     while(repaint){
        //         repaint();
        //         try {
        //             Thread.sleep(1);
        //         } catch (InterruptedException e) {
        //         }
        //     }
        // },"GameView - Repainting").start();

        this.initilizeCanvas(width, height);
        /**
         * Fonctionalité abandonnée
         */
        // this.inGameView.add(lootMenuPanel); //Ajout du menu des loot.
        this.add(backpackmenu, 0);

        Integer [] coords = {
            Config.adaptValueToScreen(30, 1),
            Config.screenH - Config.adaptValueToScreen(100, -1)
        };
        this.barrierEnergyPanel = new TextPopUp("Energie : 0", Color.white, 10, Ressources.PATH+"null.png", coords);
        
    }

    public void setDecalageY(int y){
       inGameView.decalageY = y;
       inGameView.refreshGraphics();
    }

    /**
     * Cette fonction va initialiser tous les panneaux du jeu qui vonte servir de calques pour les différents éléments du jeu,  
     * tels que les entités les murs, les effets visuels, etc.
     * <p>Tous ces panneaux seront contenu dans {@link InGameView}, ainsi ils auront les mêmes dimensions que le inGameView.
     * 
     * @param dX Le décalage dans l'axe X du panneau.
     * @param dY Le décalage dans l'axe Y du panneau.
     * @param w La largeur des panneaux.
     * @param h La hauteur des panneaux.
     */
    private void initilizeCanvas(int w, int h){
        initWallsPanel(w, h);
        initItemPanel(w, h);
        initEntityPanel(w, h); //Création de du panneau des entités.
        initFxPanel(w, h);
        initScopePanel();
    }   
            /**
             * Fonction qui initialise le panneau affichant le facteur de zoom de la carte.
             */
            private void initScopePanel(){
                this.scopePanel = new ScopePanel();
                this.add(scopePanel, 0);
                this.add(scopePanel.scopeIcon.textScope, 0);
            }


            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             * 
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initEntityPanel(int w, int h){
                this.entityPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.entityPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.entityPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.entityPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.entityPanel,0); //Ajout du panneau dans l'interface de jeu.
            }
            
            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initItemPanel(int w, int h){
                this.itemPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.itemPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.itemPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.itemPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.itemPanel,0); //Ajout du panneau dans l'interface de jeu.
            }

            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initWallsPanel(int w, int h){
                this.wallsPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.wallsPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.wallsPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.wallsPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.wallsPanel,0); //Ajout du panneau dans l'interface de jeu.
            }

            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initFxPanel(int w, int h){
                this.fxPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.fxPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.fxPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.fxPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.fxPanel,0); //Ajout du panneau dans l'interface de jeu.
            }





    /**
     * C'est cette Objet de l'interface graphique qui va contenir tous les éléments du jeu comme le joueurs, les énnemis.
     */
    public class InGameView extends JPanel implements Refreshable{

        /**
         * Il s'agit ici d'un décalage, vers la gauche du panneau.
         * <p>
         * <strong>Explication :</strong> La carte du jeu sera faite de manière à avoir un peu plus d'éléments en dehors
         *  de l'écran (C'est à dire en gros que certains éléments seront en dehors de l'écran, exprès). Cela permettera 
         * de faire l'implémentation de fonctionnalités très intéressantes dans le jeu, comme par exemple, des effets de 
         * tremblement de l'écran (Shake), de plus en plus grand pour simuler des effet de caméra lors du tir.
         * 
         * Mais également cela permettra de rajouter des fonctionnalités comme les zooms avec des armes de précision, 
         * (sniper), dans lesquels la map deviendra de plus petite et affichera plus d'éléments, selon le type de zoom
         * 
         * Le décalage en X est alors le décalage vers la gauche qui ferra que le joueur verra ce qui est au centre, mais 
         * pas les alentours.
         */
        private int decalageX; 

        /**
         * Il s'agit ici d'un décalage, vers le bas du panneau.
         * <p>
         * <strong>Explication :</strong> La carte du jeu sera faite de manière à avoir un peu plus d'éléments en dehors
         *  de l'écran (C'est à dire en gros que certains éléments seront en dehors de l'écran, exprès). Cela permettera 
         * de faire l'implémentation de fonctionnalités très intéressantes dans le jeu, comme par exemple, des effets de 
         * tremblement de l'écran (Shake), de plus en plus grand pour simuler des effet de caméra lors du tir.
         * 
         * (Le mode sniper ne changera pas la vue de haut en bas mais uniquement de droite à gauche)
         * 
         * Le décalage en Y est alors le décalage vers le bas qui ferra que le joueur verra ce qui est au centre, mais 
         * pas les alentours.
         */
        private int decalageY;

        /**
         * Panneau qui contiendera l'image d'arrière plan du jeu.
         */
        private JPanel background;

        /**
         * Booléen utilisé pour faire fonctionner le Thread qui faitb bouger la carte.
         * <p>Tant qu'il vaut {@code true} le background du jeu sera animé.
         */
        private boolean backgroundThreadactive = true;

        /**
         * Eniter représentant le décalage à ajouter à l'arrière plan pour le décaler en haut ou en bas.
         */
        private int bacgroundDY = 1;

        /**
         * Constructeur de l'interface, il prend en argument les dimension de l'interface
         */
        InGameView(int decalageX, int decalageY, int width, int height, String backgroundLocation){
            this.decalageX = decalageX; //Mise à jour du décalage en X
            this.decalageY = decalageY; //Mise à jour du décalage en Y
            this.setBounds( //Mise à l'échelle de l'interface de jeu
                decalageX,
                decalageY,
                width,
                height
            );
            this.setLayout(null); //Pas de Layout
            this.setFocusable(true);
            this.setOpaque(false); //Activation de la transparence
            // this.setBackground(new Color(200, 111, 33));
            this.initBackground(backgroundLocation);
            this.add(background);
            this.makeThreadBackgroundMoving();
            this.addMouseWheelActions();
            // System.out.println("inGameView : [X = "+decalageX+", Y = "+decalageY+", Width = "+width+", Height = "+height); //debug
        }

                /**
                 * Fonction qui ajoute les actions à faire lorsque molette de la souris est enclenchée.
                 */
                private void addMouseWheelActions(){

                    this.addMouseWheelListener(new MouseWheelListener() {

                        @Override
                        public void mouseWheelMoved(MouseWheelEvent e){
                            //Délai pour éviter que l'intervalle d'exécution de la fonction ne soit trop rapide.
                                // if (System.currentTimeMillis() - delay < 200)
                                //     return;
                                // delay = System.currentTimeMillis();
                            /////////////////////////////////


                            int dir = e.getWheelRotation(); //Direction actuelle de la rotation de la molette.

                            if (dir < 0) { 
                                //"Molette tournée vers le haut"
                                // System.out.println("Haut");
                                if(GameView.this.map.getFov()>12) {//Si le FOV est au dessus de douze.
                                    GameView.this.map.resetFov(GameView.this.map.getFov()-1);
                                    updateScopePanel();
                                }
                                
                            } else {
                                //"Molette tournée vers le bas"
                                // System.out.println("bas");
                                if(GameView.this.map.getFov()<20) //Si le FOV est en dessous de vingt.
                                    GameView.this.map.resetFov(GameView.this.map.getFov()+1);
                                    updateScopePanel();
                            }

                        }
                    });

                }

                    private void updateScopePanel(){
                        
                        int fov = GameView.this.map.getFov();
                        int percentage = (fov-12) * 100 / 8;
                        System.out.println(percentage);
                        int scopeBarHeight = ((GameView)Config.window.getContentPane()).getScopePanel().scopeManage.getHeight();
                        int Ysep = scopeBarHeight * percentage / 100;
                        ScopeIcon sI = ((GameView)Config.window.getContentPane()).getScopePanel().scopeIcon;
                        ScopeManage sM = ((GameView)Config.window.getContentPane()).getScopePanel().scopeManage;
                        sI.setLocation(sI.getX(), Ysep + Config.adaptValueToScreen(80, -1) - sI.getHeight()/2);
                        sM.updateYSep(Ysep);
                    }


                private void initBackground(String backgroundLocation){
                    this.background = new JPanel(){

                        private Image background = new ImageIcon(backgroundLocation).getImage();

                        {

                            this.setBounds(
                                0,
                                Config.adaptValueToScreen(-10000, 1),
                                InGameView.this.getWidth(),
                                InGameView.this.getHeight() + Config.adaptValueToScreen(10000, -1)
                            );
                            this.setOpaque(false);

                        }

                        @Override
                        public void paintComponent(Graphics g){
                            Graphics2D g2d = (Graphics2D)g;
                            g2d.drawImage(background, 0, 0, this.getWidth(), this.getHeight(), null);
                            super.paintComponent(g);
                        }

                    };
                }
                
                /**
                 * Cette fonction lance un Thread qui va faire descendre le background vers le bas en permanence.
                 */
                private void makeThreadBackgroundMoving(){
                    Thread thread = 
                        new Thread(
                            ()->{
                                while(backgroundThreadactive && GameUpdater.isThreadWorking){ //Tant qu'il est possible d'animer l'arrière plan.
                                    try{
                                        updateBackgroundLocation();
                                        Thread.sleep(20);
                                    }catch(InterruptedException e){
                                        System.out.println("Thread d'animation d'arrière plan stoppé !");
                                    }
                                    catch(IllegalComponentStateException e){
                                        //Ce n'est pas grave, cela est due au fait que tous les éléments ne sont pas encore initilisés au début du Thread.
                                    }
                                }
                            },
                            "GameView - Background Moving Thread"
                        );
                        
                    Config.toInterupt.add(thread);
                    thread.start();
                }

                        /**
                         * Fonction qui mets à jour la position de l'arrière plan pour faire l'animation.
                         * @throws IllegalComponentStateException Lorsque tous les éléments ne sont pas encore initilisés au début du Thread.
                         */
                        private void updateBackgroundLocation() throws IllegalComponentStateException{
                            
                            if(GameView.this.getLocationOnScreen().y <= this.background.getLocationOnScreen().y){
                                this.background.setBounds(
                                    0,
                                    this.getY() + Config.adaptValueToScreen(-10000, -1),
                                    this.background.getWidth(),
                                    this.background.getHeight()
                                );
                            }
                            else{
                                this.background.setBounds(
                                    0,
                                    this. background.getY() + this.bacgroundDY,
                                    this.background.getWidth(),
                                    this.background.getHeight()
                                );
                            }

                        }

    
        /**
         * Fonction qui va rafraichir la dimension des éléments à l'écran, si jamais l'écran est redimensionné pendant la 
         * partie
         */
        @Override
        public void refreshGraphics(){
            this.setBounds( //Mise à l'échelle de l'interface de jeu
                Config.adaptValueToScreen(decalageX, 1), 
                Config.adaptValueToScreen(decalageY, -1),
                Config.adaptValueToScreen(this.getWidth(), 1),
                Config.adaptValueToScreen(this.getHeight(), -1)
            );
            this.setLayout(null); //Pas de Layout
        }
        
        /**
         * Fonction qui arrête l'animation de l'arrière plan du jeu.
         */
        public void stopBackgroundAnime(){
            this.backgroundThreadactive = false;
        }

        /**
         * Fonction qui revoie la valeur actuelle du décalage de l'arrière plan.
         * @return Décalage de l'arrière plan.
         */
        public int getBackgroundDY(){
            return this.bacgroundDY;
        }
        
        /**
         * Fonction qui change le décalage de l'arrière plan.
         * @param bacgroundDY Nouveau décalage de l'arrière plan
         */
        public void setBackgroundDY(int bacgroundDY){
            this. bacgroundDY= bacgroundDY;
        }
    }


    /**
     * Fonction qui va rafraichir la dimension des éléments à l'écran, si jamais l'écran est redimensionné pendant la 
     * partie
     */
    @Override
    public void refreshGraphics(){
        this.setBounds(0,0,Config.screenW, Config.screenH); //Remise à l'échelle du GameView.
        this.inGameView.refreshGraphics(); //Remise à l'echelle du JPanel contenant tous les éléments du jeu.
    }
  
    /**
     * Getter de l'interface de jeu
     */
    public InGameView getInGameView(){return this.inGameView;}

    public ArmingInterface getArmingInterface(){
        return this.armingPanel;
    }

    

    /**
     * Fonction qui renvoi le panneau des entités.
     * <p>Va servir lors de l'ajout des entités dans le jeu tels que le joueur ou les ennemis.
     * @return Le panneau des entités dans le jeu.
     */
    public JPanel getEntityPanel(){
        return this.entityPanel;
    }

    /**
     * Fonction qui renvoi le panneau des items du jeu.
     * <p>Va servir lors de l'ajout des items dans le jeu.
     * @return Le panneau des items dans le jeu.
     */
    public JPanel getItemPanel(){
        return this.itemPanel;
    }

    /**
     * Fonction qui renvoi le panneau des murs du jeu.
     * <p>Va servir lors de l'ajout des murs dans le jeu.
     * @return Le panneau des murs dans le jeu.
     */
    public JPanel getWallsPanel(){
        return this.wallsPanel;
    }

    /**
     * Fonction qui renvoi le panneau des effets visuels du jeu.
     * <p>Va servir lors de l'ajout des effets visuels dans  le jeu.
     * @return Le panneau des effets visuels du jeu.
     */
    public JPanel getFxPanel(){
        return this.fxPanel;
    }

    /**
     * Fonction qui renvoi le panneau de l'inventaire du joueur. 
     * @return Le panneau de l'inventaire.
     */
    public BackPackPanel getBackPackPanel(){
        return this.backpackmenu;
    }

    /**
     * Fonction qui renvoie la référence de la carte.
     * @return La référence de la carte.
     */
    public Map getMap(){return this.map;}

    /**
     * Fonction qui renvoie le panneau affichant le facteur d'echelle de la carte.
     * @return Le panneau affichant le facteur d'echelle de la carte.
     */
    public ScopePanel getScopePanel(){return this.scopePanel;}


    /**
     * Fonction qui change la réfénrence du joueur.
     * @param player la nouvelle référence du joueur.
     */
    public void setBackPackMenuPlayer(Player player){
        this.backpackmenu.setPlayer(player);
    }


    /**
     * Fonction qui met à jour la référence de la carte actuelle du jeu.
     * @param map La nouvelle carte.
     */
    public void setMap(Map map){
        this.map = map;
    }


    //Fonctionnalité abandonnée
    // /**
    //  * Fonction qui renvoi le menu des items.
    //  * @return Le menu des items.
    //  * @see LootMenu
    //  */
    // public LootMenu getLootMenuPanel() {
    //     return lootMenuPanel;
    // }
    
    //Fonctionnalité abandonnée
    // /**
    //  * 
    //  * Fonction qui change le menu des item.
    //  * @return Le nouveau menu des items.
    //  * @see LootMenu
    //  */
    // @Deprecated
    // public void setLootMenuPanel(LootMenu lootMenuPanel) {
    //     this.lootMenuPanel = lootMenuPanel;
    // }

}
package gui.gameview;

import java.awt.Color;
import java.awt.Dimension;

///////////////////////////   [==  Star Quest  ==]   ///////////////////////////////
//  [IMPORTANT] Pour une meilleur lisibilité, masquer les pavés de commentaire avec l'IDE, ces commentaires sont surtout reservés à la documentation HTML (Javadoc).
//////////////////////////////////////////////////////////////////////////////////////////////////////////

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.IllegalComponentStateException;
import java.awt.Image;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import javax.swing.ImageIcon;
import javax.swing.JPanel;

import controller.Config;
import controller.GameUpdater;
import gui.Refreshable;
import gui.gameview.backpackpanel.BackPackPanel;
import gui.gameview.scope.ScopeIcon;
import gui.gameview.scope.ScopeManage;
import gui.gameview.scope.ScopePanel;
import gui.gameview.textpopup.TextPopUp;
import model.Player;
import model.map.Map;
import model.memRes.Ressources;

/**
 * Cette classe représente l'interface de jeu à part entière, elle va contenir le HUD (les boutons, la barre de vie, etc), 
 * ainsi que tous les éléments du jeu comme, le joueur, le ennemis,etc
 */
public class GameView extends JPanel implements Refreshable{

    /**
     * l'interface graphique qui va contenir toutes les objet de jeu jeu comme le joueurs, les énnemis, etc
     */
    private InGameView inGameView;

    /**
     * Cet attribut sera utilisé dans un Thread qui va repaindre en permanence le JPanel pour éviter les effets de glich à l'écran.
     */
    public boolean repaint = true;

    /**
     * Panneau donnant des informations sur les armes.
     */
    private ArmingInterface armingPanel = new ArmingInterface();

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les entités tels que les ennemis et le joueur.
     */
    private JPanel entityPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les items du jeu.
     */
    private JPanel itemPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les Murs du jeu.
     */
    private JPanel wallsPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les effets visuels du jeu.
     */
    private JPanel fxPanel;

    /**
     * Il s'agit du menu de l'inventaire du joueur.
     */
    private BackPackPanel backpackmenu;

    /**
     * Il s'agit de la référence à la carte actuel de la partie.
     */
    private Map map;

    /**
     * Il s'agit du panneau affichant la valeur actuelle du facteur d'echelle de la carte.
     */
    private ScopePanel scopePanel = new ScopePanel();
    
    private TextPopUp barrierEnergyPanel;

    ///**
    // * Fonctionalité abandonnée
    // */
    ///**
    // * Paneau affichant le menu des item à ramasser.
    // */
    // private LootMenu lootMenuPanel = new LootMenu();
    
    /**
     * Constructeur de GameView, il prend en argument les dimension de l'interface de jeu (InGameView)
     */
    public GameView(int decalageX, int decalageY, int width, int height, String bakcgroundLocation){
        Config.window.setContentPane(this); //GameView devient le paneau principale de l'interface.
        this.setLayout(null); //GameView n'a pas de Layout.
        this.inGameView = new InGameView(decalageX, decalageY, width, height, bakcgroundLocation); //Création d'une view du jeu, 
        this.add(inGameView); //Ajout dans l'interface de jeu dans l'interface global GameView.
        this.add(this.armingPanel,0);
        Config.elementRefreshables.add(this); //Ajout de l'interface de jeu comme étant un refreshable, dans Config.
        Config.updatable.add(this);
        this.backpackmenu = new BackPackPanel(null);
        this.setBounds(0,0,Config.screenW, Config.screenH);
        this.setPreferredSize(new Dimension(Config.screenW, Config.screenH));

        // new Thread(()->{
        //     while(repaint){
        //         repaint();
        //         try {
        //             Thread.sleep(1);
        //         } catch (InterruptedException e) {
        //         }
        //     }
        // },"GameView - Repainting").start();

        this.initilizeCanvas(width, height);
        /**
         * Fonctionalité abandonnée
         */
        // this.inGameView.add(lootMenuPanel); //Ajout du menu des loot.
        this.add(backpackmenu, 0);

        Integer [] coords = {
            Config.adaptValueToScreen(30, 1),
            Config.screenH - Config.adaptValueToScreen(100, -1)
        };
        this.barrierEnergyPanel = new TextPopUp("Energie : 0", Color.white, 10, Ressources.PATH+"null.png", coords);
        
    }

    public void setDecalageY(int y){
       inGameView.decalageY = y;
       inGameView.refreshGraphics();
    }

    /**
     * Cette fonction va initialiser tous les panneaux du jeu qui vonte servir de calques pour les différents éléments du jeu,  
     * tels que les entités les murs, les effets visuels, etc.
     * <p>Tous ces panneaux seront contenu dans {@link InGameView}, ainsi ils auront les mêmes dimensions que le inGameView.
     * 
     * @param dX Le décalage dans l'axe X du panneau.
     * @param dY Le décalage dans l'axe Y du panneau.
     * @param w La largeur des panneaux.
     * @param h La hauteur des panneaux.
     */
    private void initilizeCanvas(int w, int h){
        initWallsPanel(w, h);
        initItemPanel(w, h);
        initEntityPanel(w, h); //Création de du panneau des entités.
        initFxPanel(w, h);
        initScopePanel();
    }   
            /**
             * Fonction qui initialise le panneau affichant le facteur de zoom de la carte.
             */
            private void initScopePanel(){
                this.scopePanel = new ScopePanel();
                this.add(scopePanel, 0);
                this.add(scopePanel.scopeIcon.textScope, 0);
            }


            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             * 
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initEntityPanel(int w, int h){
                this.entityPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.entityPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.entityPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.entityPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.entityPanel,0); //Ajout du panneau dans l'interface de jeu.
            }
            
            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initItemPanel(int w, int h){
                this.itemPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.itemPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.itemPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.itemPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.itemPanel,0); //Ajout du panneau dans l'interface de jeu.
            }

            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initWallsPanel(int w, int h){
                this.wallsPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.wallsPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.wallsPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.wallsPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.wallsPanel,0); //Ajout du panneau dans l'interface de jeu.
            }

            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initFxPanel(int w, int h){
                this.fxPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.fxPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.fxPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.fxPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.fxPanel,0); //Ajout du panneau dans l'interface de jeu.
            }





    /**
     * C'est cette Objet de l'interface graphique qui va contenir tous les éléments du jeu comme le joueurs, les énnemis.
     */
    public class InGameView extends JPanel implements Refreshable{

        /**
         * Il s'agit ici d'un décalage, vers la gauche du panneau.
         * <p>
         * <strong>Explication :</strong> La carte du jeu sera faite de manière à avoir un peu plus d'éléments en dehors
         *  de l'écran (C'est à dire en gros que certains éléments seront en dehors de l'écran, exprès). Cela permettera 
         * de faire l'implémentation de fonctionnalités très intéressantes dans le jeu, comme par exemple, des effets de 
         * tremblement de l'écran (Shake), de plus en plus grand pour simuler des effet de caméra lors du tir.
         * 
         * Mais également cela permettra de rajouter des fonctionnalités comme les zooms avec des armes de précision, 
         * (sniper), dans lesquels la map deviendra de plus petite et affichera plus d'éléments, selon le type de zoom
         * 
         * Le décalage en X est alors le décalage vers la gauche qui ferra que le joueur verra ce qui est au centre, mais 
         * pas les alentours.
         */
        private int decalageX; 

        /**
         * Il s'agit ici d'un décalage, vers le bas du panneau.
         * <p>
         * <strong>Explication :</strong> La carte du jeu sera faite de manière à avoir un peu plus d'éléments en dehors
         *  de l'écran (C'est à dire en gros que certains éléments seront en dehors de l'écran, exprès). Cela permettera 
         * de faire l'implémentation de fonctionnalités très intéressantes dans le jeu, comme par exemple, des effets de 
         * tremblement de l'écran (Shake), de plus en plus grand pour simuler des effet de caméra lors du tir.
         * 
         * (Le mode sniper ne changera pas la vue de haut en bas mais uniquement de droite à gauche)
         * 
         * Le décalage en Y est alors le décalage vers le bas qui ferra que le joueur verra ce qui est au centre, mais 
         * pas les alentours.
         */
        private int decalageY;

        /**
         * Panneau qui contiendera l'image d'arrière plan du jeu.
         */
        private JPanel background;

        /**
         * Booléen utilisé pour faire fonctionner le Thread qui faitb bouger la carte.
         * <p>Tant qu'il vaut {@code true} le background du jeu sera animé.
         */
        private boolean backgroundThreadactive = true;

        /**
         * Eniter représentant le décalage à ajouter à l'arrière plan pour le décaler en haut ou en bas.
         */
        private int bacgroundDY = 1;

        /**
         * Constructeur de l'interface, il prend en argument les dimension de l'interface
         */
        InGameView(int decalageX, int decalageY, int width, int height, String backgroundLocation){
            this.decalageX = decalageX; //Mise à jour du décalage en X
            this.decalageY = decalageY; //Mise à jour du décalage en Y
            this.setBounds( //Mise à l'échelle de l'interface de jeu
                decalageX,
                decalageY,
                width,
                height
            );
            this.setLayout(null); //Pas de Layout
            this.setFocusable(true);
            this.setOpaque(false); //Activation de la transparence
            // this.setBackground(new Color(200, 111, 33));
            this.initBackground(backgroundLocation);
            this.add(background);
            this.makeThreadBackgroundMoving();
            this.addMouseWheelActions();
            // System.out.println("inGameView : [X = "+decalageX+", Y = "+decalageY+", Width = "+width+", Height = "+height); //debug
        }

                /**
                 * Fonction qui ajoute les actions à faire lorsque molette de la souris est enclenchée.
                 */
                private void addMouseWheelActions(){

                    this.addMouseWheelListener(new MouseWheelListener() {

                        @Override
                        public void mouseWheelMoved(MouseWheelEvent e){
                            //Délai pour éviter que l'intervalle d'exécution de la fonction ne soit trop rapide.
                                // if (System.currentTimeMillis() - delay < 200)
                                //     return;
                                // delay = System.currentTimeMillis();
                            /////////////////////////////////


                            int dir = e.getWheelRotation(); //Direction actuelle de la rotation de la molette.

                            if (dir < 0) { 
                                //"Molette tournée vers le haut"
                                // System.out.println("Haut");
                                if(GameView.this.map.getFov()>12) {//Si le FOV est au dessus de douze.
                                    GameView.this.map.resetFov(GameView.this.map.getFov()-1);
                                    updateScopePanel();
                                }
                                
                            } else {
                                //"Molette tournée vers le bas"
                                // System.out.println("bas");
                                if(GameView.this.map.getFov()<20) //Si le FOV est en dessous de vingt.
                                    GameView.this.map.resetFov(GameView.this.map.getFov()+1);
                                    updateScopePanel();
                            }

                        }
                    });

                }

                    private void updateScopePanel(){
                        
                        int fov = GameView.this.map.getFov();
                        int percentage = (fov-12) * 100 / 8;
                        System.out.println(percentage);
                        int scopeBarHeight = ((GameView)Config.window.getContentPane()).getScopePanel().scopeManage.getHeight();
                        int Ysep = scopeBarHeight * percentage / 100;
                        ScopeIcon sI = ((GameView)Config.window.getContentPane()).getScopePanel().scopeIcon;
                        ScopeManage sM = ((GameView)Config.window.getContentPane()).getScopePanel().scopeManage;
                        sI.setLocation(sI.getX(), Ysep + Config.adaptValueToScreen(80, -1) - sI.getHeight()/2);
                        sM.updateYSep(Ysep);
                    }


                private void initBackground(String backgroundLocation){
                    this.background = new JPanel(){

                        private Image background = new ImageIcon(backgroundLocation).getImage();

                        {

                            this.setBounds(
                                0,
                                Config.adaptValueToScreen(-10000, 1),
                                InGameView.this.getWidth(),
                                InGameView.this.getHeight() + Config.adaptValueToScreen(10000, -1)
                            );
                            this.setOpaque(false);

                        }

                        @Override
                        public void paintComponent(Graphics g){
                            Graphics2D g2d = (Graphics2D)g;
                            g2d.drawImage(background, 0, 0, this.getWidth(), this.getHeight(), null);
                            super.paintComponent(g);
                        }

                    };
                }
                
                /**
                 * Cette fonction lance un Thread qui va faire descendre le background vers le bas en permanence.
                 */
                private void makeThreadBackgroundMoving(){
                    Thread thread = 
                        new Thread(
                            ()->{
                                while(backgroundThreadactive && GameUpdater.isThreadWorking){ //Tant qu'il est possible d'animer l'arrière plan.
                                    try{
                                        updateBackgroundLocation();
                                        Thread.sleep(20);
                                    }catch(InterruptedException e){
                                        System.out.println("Thread d'animation d'arrière plan stoppé !");
                                    }
                                    catch(IllegalComponentStateException e){
                                        //Ce n'est pas grave, cela est due au fait que tous les éléments ne sont pas encore initilisés au début du Thread.
                                    }
                                }
                            },
                            "GameView - Background Moving Thread"
                        );
                        
                    Config.toInterupt.add(thread);
                    thread.start();
                }

                        /**
                         * Fonction qui mets à jour la position de l'arrière plan pour faire l'animation.
                         * @throws IllegalComponentStateException Lorsque tous les éléments ne sont pas encore initilisés au début du Thread.
                         */
                        private void updateBackgroundLocation() throws IllegalComponentStateException{
                            
                            if(GameView.this.getLocationOnScreen().y <= this.background.getLocationOnScreen().y){
                                this.background.setBounds(
                                    0,
                                    this.getY() + Config.adaptValueToScreen(-10000, -1),
                                    this.background.getWidth(),
                                    this.background.getHeight()
                                );
                            }
                            else{
                                this.background.setBounds(
                                    0,
                                    this. background.getY() + this.bacgroundDY,
                                    this.background.getWidth(),
                                    this.background.getHeight()
                                );
                            }

                        }

    
        /**
         * Fonction qui va rafraichir la dimension des éléments à l'écran, si jamais l'écran est redimensionné pendant la 
         * partie
         */
        @Override
        public void refreshGraphics(){
            this.setBounds( //Mise à l'échelle de l'interface de jeu
                Config.adaptValueToScreen(decalageX, 1), 
                Config.adaptValueToScreen(decalageY, -1),
                Config.adaptValueToScreen(this.getWidth(), 1),
                Config.adaptValueToScreen(this.getHeight(), -1)
            );
            this.setLayout(null); //Pas de Layout
        }
        
        /**
         * Fonction qui arrête l'animation de l'arrière plan du jeu.
         */
        public void stopBackgroundAnime(){
            this.backgroundThreadactive = false;
        }

        /**
         * Fonction qui revoie la valeur actuelle du décalage de l'arrière plan.
         * @return Décalage de l'arrière plan.
         */
        public int getBackgroundDY(){
            return this.bacgroundDY;
        }
        
        /**
         * Fonction qui change le décalage de l'arrière plan.
         * @param bacgroundDY Nouveau décalage de l'arrière plan
         */
        public void setBackgroundDY(int bacgroundDY){
            this. bacgroundDY= bacgroundDY;
        }
    }


    /**
     * Fonction qui va rafraichir la dimension des éléments à l'écran, si jamais l'écran est redimensionné pendant la 
     * partie
     */
    @Override
    public void refreshGraphics(){
        this.setBounds(0,0,Config.screenW, Config.screenH); //Remise à l'échelle du GameView.
        this.inGameView.refreshGraphics(); //Remise à l'echelle du JPanel contenant tous les éléments du jeu.
    }
  
    /**
     * Getter de l'interface de jeu
     */
    public InGameView getInGameView(){return this.inGameView;}

    public ArmingInterface getArmingInterface(){
        return this.armingPanel;
    }

    

    /**
     * Fonction qui renvoi le panneau des entités.
     * <p>Va servir lors de l'ajout des entités dans le jeu tels que le joueur ou les ennemis.
     * @return Le panneau des entités dans le jeu.
     */
    public JPanel getEntityPanel(){
        return this.entityPanel;
    }

    /**
     * Fonction qui renvoi le panneau des items du jeu.
     * <p>Va servir lors de l'ajout des items dans le jeu.
     * @return Le panneau des items dans le jeu.
     */
    public JPanel getItemPanel(){
        return this.itemPanel;
    }

    /**
     * Fonction qui renvoi le panneau des murs du jeu.
     * <p>Va servir lors de l'ajout des murs dans le jeu.
     * @return Le panneau des murs dans le jeu.
     */
    public JPanel getWallsPanel(){
        return this.wallsPanel;
    }

    /**
     * Fonction qui renvoi le panneau des effets visuels du jeu.
     * <p>Va servir lors de l'ajout des effets visuels dans  le jeu.
     * @return Le panneau des effets visuels du jeu.
     */
    public JPanel getFxPanel(){
        return this.fxPanel;
    }

    /**
     * Fonction qui renvoi le panneau de l'inventaire du joueur. 
     * @return Le panneau de l'inventaire.
     */
    public BackPackPanel getBackPackPanel(){
        return this.backpackmenu;
    }

    /**
     * Fonction qui renvoie la référence de la carte.
     * @return La référence de la carte.
     */
    public Map getMap(){return this.map;}

    /**
     * Fonction qui renvoie le panneau affichant le facteur d'echelle de la carte.
     * @return Le panneau affichant le facteur d'echelle de la carte.
     */
    public ScopePanel getScopePanel(){return this.scopePanel;}


    /**
     * Fonction qui change la réfénrence du joueur.
     * @param player la nouvelle référence du joueur.
     */
    public void setBackPackMenuPlayer(Player player){
        this.backpackmenu.setPlayer(player);
    }


    /**
     * Fonction qui met à jour la référence de la carte actuelle du jeu.
     * @param map La nouvelle carte.
     */
    public void setMap(Map map){
        this.map = map;
    }


    //Fonctionnalité abandonnée
    // /**
    //  * Fonction qui renvoi le menu des items.
    //  * @return Le menu des items.
    //  * @see LootMenu
    //  */
    // public LootMenu getLootMenuPanel() {
    //     return lootMenuPanel;
    // }
    
    //Fonctionnalité abandonnée
    // /**
    //  * 
    //  * Fonction qui change le menu des item.
    //  * @return Le nouveau menu des items.
    //  * @see LootMenu
    //  */
    // @Deprecated
    // public void setLootMenuPanel(LootMenu lootMenuPanel) {
    //     this.lootMenuPanel = lootMenuPanel;
    // }

}
package gui.gameview;

import java.awt.Color;
import java.awt.Dimension;

///////////////////////////   [==  Star Quest  ==]   ///////////////////////////////
//  [IMPORTANT] Pour une meilleur lisibilité, masquer les pavés de commentaire avec l'IDE, ces commentaires sont surtout reservés à la documentation HTML (Javadoc).
//////////////////////////////////////////////////////////////////////////////////////////////////////////

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.IllegalComponentStateException;
import java.awt.Image;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import javax.swing.ImageIcon;
import javax.swing.JPanel;

import controller.Config;
import controller.GameUpdater;
import gui.Refreshable;
import gui.gameview.backpackpanel.BackPackPanel;
import gui.gameview.scope.ScopeIcon;
import gui.gameview.scope.ScopeManage;
import gui.gameview.scope.ScopePanel;
import gui.gameview.textpopup.TextPopUp;
import model.Player;
import model.map.Map;
import model.memRes.Ressources;

/**
 * Cette classe représente l'interface de jeu à part entière, elle va contenir le HUD (les boutons, la barre de vie, etc), 
 * ainsi que tous les éléments du jeu comme, le joueur, le ennemis,etc
 */
public class GameView extends JPanel implements Refreshable{

    /**
     * l'interface graphique qui va contenir toutes les objet de jeu jeu comme le joueurs, les énnemis, etc
     */
    private InGameView inGameView;

    /**
     * Cet attribut sera utilisé dans un Thread qui va repaindre en permanence le JPanel pour éviter les effets de glich à l'écran.
     */
    public boolean repaint = true;

    /**
     * Panneau donnant des informations sur les armes.
     */
    private ArmingInterface armingPanel = new ArmingInterface();

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les entités tels que les ennemis et le joueur.
     */
    private JPanel entityPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les items du jeu.
     */
    private JPanel itemPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les Murs du jeu.
     */
    private JPanel wallsPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les effets visuels du jeu.
     */
    private JPanel fxPanel;

    /**
     * Il s'agit du menu de l'inventaire du joueur.
     */
    private BackPackPanel backpackmenu;

    /**
     * Il s'agit de la référence à la carte actuel de la partie.
     */
    private Map map;

    /**
     * Il s'agit du panneau affichant la valeur actuelle du facteur d'echelle de la carte.
     */
    private ScopePanel scopePanel = new ScopePanel();
    
    private TextPopUp barrierEnergyPanel;

    ///**
    // * Fonctionalité abandonnée
    // */
    ///**
    // * Paneau affichant le menu des item à ramasser.
    // */
    // private LootMenu lootMenuPanel = new LootMenu();
    
    /**
     * Constructeur de GameView, il prend en argument les dimension de l'interface de jeu (InGameView)
     */
    public GameView(int decalageX, int decalageY, int width, int height, String bakcgroundLocation){
        Config.window.setContentPane(this); //GameView devient le paneau principale de l'interface.
        this.setLayout(null); //GameView n'a pas de Layout.
        this.inGameView = new InGameView(decalageX, decalageY, width, height, bakcgroundLocation); //Création d'une view du jeu, 
        this.add(inGameView); //Ajout dans l'interface de jeu dans l'interface global GameView.
        this.add(this.armingPanel,0);
        Config.elementRefreshables.add(this); //Ajout de l'interface de jeu comme étant un refreshable, dans Config.
        Config.updatable.add(this);
        this.backpackmenu = new BackPackPanel(null);
        this.setBounds(0,0,Config.screenW, Config.screenH);
        this.setPreferredSize(new Dimension(Config.screenW, Config.screenH));

        // new Thread(()->{
        //     while(repaint){
        //         repaint();
        //         try {
        //             Thread.sleep(1);
        //         } catch (InterruptedException e) {
        //         }
        //     }
        // },"GameView - Repainting").start();

        this.initilizeCanvas(width, height);
        /**
         * Fonctionalité abandonnée
         */
        // this.inGameView.add(lootMenuPanel); //Ajout du menu des loot.
        this.add(backpackmenu, 0);

        Integer [] coords = {
            Config.adaptValueToScreen(30, 1),
            Config.screenH - Config.adaptValueToScreen(100, -1)
        };
        this.barrierEnergyPanel = new TextPopUp("Energie : 0", Color.white, 10, Ressources.PATH+"null.png", coords);
        
    }

    public void setDecalageY(int y){
       inGameView.decalageY = y;
       inGameView.refreshGraphics();
    }

    /**
     * Cette fonction va initialiser tous les panneaux du jeu qui vonte servir de calques pour les différents éléments du jeu,  
     * tels que les entités les murs, les effets visuels, etc.
     * <p>Tous ces panneaux seront contenu dans {@link InGameView}, ainsi ils auront les mêmes dimensions que le inGameView.
     * 
     * @param dX Le décalage dans l'axe X du panneau.
     * @param dY Le décalage dans l'axe Y du panneau.
     * @param w La largeur des panneaux.
     * @param h La hauteur des panneaux.
     */
    private void initilizeCanvas(int w, int h){
        initWallsPanel(w, h);
        initItemPanel(w, h);
        initEntityPanel(w, h); //Création de du panneau des entités.
        initFxPanel(w, h);
        initScopePanel();
    }   
            /**
             * Fonction qui initialise le panneau affichant le facteur de zoom de la carte.
             */
            private void initScopePanel(){
                this.scopePanel = new ScopePanel();
                this.add(scopePanel, 0);
                this.add(scopePanel.scopeIcon.textScope, 0);
            }


            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             * 
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initEntityPanel(int w, int h){
                this.entityPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.entityPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.entityPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.entityPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.entityPanel,0); //Ajout du panneau dans l'interface de jeu.
            }
            
            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initItemPanel(int w, int h){
                this.itemPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.itemPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.itemPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.itemPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.itemPanel,0); //Ajout du panneau dans l'interface de jeu.
            }

            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initWallsPanel(int w, int h){
                this.wallsPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.wallsPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.wallsPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.wallsPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.wallsPanel,0); //Ajout du panneau dans l'interface de jeu.
            }

            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initFxPanel(int w, int h){
                this.fxPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.fxPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.fxPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.fxPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.fxPanel,0); //Ajout du panneau dans l'interface de jeu.
            }





    /**
     * C'est cette Objet de l'interface graphique qui va contenir tous les éléments du jeu comme le joueurs, les énnemis.
     */
    public class InGameView extends JPanel implements Refreshable{

        /**
         * Il s'agit ici d'un décalage, vers la gauche du panneau.
         * <p>
         * <strong>Explication :</strong> La carte du jeu sera faite de manière à avoir un peu plus d'éléments en dehors
         *  de l'écran (C'est à dire en gros que certains éléments seront en dehors de l'écran, exprès). Cela permettera 
         * de faire l'implémentation de fonctionnalités très intéressantes dans le jeu, comme par exemple, des effets de 
         * tremblement de l'écran (Shake), de plus en plus grand pour simuler des effet de caméra lors du tir.
         * 
         * Mais également cela permettra de rajouter des fonctionnalités comme les zooms avec des armes de précision, 
         * (sniper), dans lesquels la map deviendra de plus petite et affichera plus d'éléments, selon le type de zoom
         * 
         * Le décalage en X est alors le décalage vers la gauche qui ferra que le joueur verra ce qui est au centre, mais 
         * pas les alentours.
         */
        private int decalageX; 

        /**
         * Il s'agit ici d'un décalage, vers le bas du panneau.
         * <p>
         * <strong>Explication :</strong> La carte du jeu sera faite de manière à avoir un peu plus d'éléments en dehors
         *  de l'écran (C'est à dire en gros que certains éléments seront en dehors de l'écran, exprès). Cela permettera 
         * de faire l'implémentation de fonctionnalités très intéressantes dans le jeu, comme par exemple, des effets de 
         * tremblement de l'écran (Shake), de plus en plus grand pour simuler des effet de caméra lors du tir.
         * 
         * (Le mode sniper ne changera pas la vue de haut en bas mais uniquement de droite à gauche)
         * 
         * Le décalage en Y est alors le décalage vers le bas qui ferra que le joueur verra ce qui est au centre, mais 
         * pas les alentours.
         */
        private int decalageY;

        /**
         * Panneau qui contiendera l'image d'arrière plan du jeu.
         */
        private JPanel background;

        /**
         * Booléen utilisé pour faire fonctionner le Thread qui faitb bouger la carte.
         * <p>Tant qu'il vaut {@code true} le background du jeu sera animé.
         */
        private boolean backgroundThreadactive = true;

        /**
         * Eniter représentant le décalage à ajouter à l'arrière plan pour le décaler en haut ou en bas.
         */
        private int bacgroundDY = 1;

        /**
         * Constructeur de l'interface, il prend en argument les dimension de l'interface
         */
        InGameView(int decalageX, int decalageY, int width, int height, String backgroundLocation){
            this.decalageX = decalageX; //Mise à jour du décalage en X
            this.decalageY = decalageY; //Mise à jour du décalage en Y
            this.setBounds( //Mise à l'échelle de l'interface de jeu
                decalageX,
                decalageY,
                width,
                height
            );
            this.setLayout(null); //Pas de Layout
            this.setFocusable(true);
            this.setOpaque(false); //Activation de la transparence
            // this.setBackground(new Color(200, 111, 33));
            this.initBackground(backgroundLocation);
            this.add(background);
            this.makeThreadBackgroundMoving();
            this.addMouseWheelActions();
            // System.out.println("inGameView : [X = "+decalageX+", Y = "+decalageY+", Width = "+width+", Height = "+height); //debug
        }

                /**
                 * Fonction qui ajoute les actions à faire lorsque molette de la souris est enclenchée.
                 */
                private void addMouseWheelActions(){

                    this.addMouseWheelListener(new MouseWheelListener() {

                        @Override
                        public void mouseWheelMoved(MouseWheelEvent e){
                            //Délai pour éviter que l'intervalle d'exécution de la fonction ne soit trop rapide.
                                // if (System.currentTimeMillis() - delay < 200)
                                //     return;
                                // delay = System.currentTimeMillis();
                            /////////////////////////////////


                            int dir = e.getWheelRotation(); //Direction actuelle de la rotation de la molette.

                            if (dir < 0) { 
                                //"Molette tournée vers le haut"
                                // System.out.println("Haut");
                                if(GameView.this.map.getFov()>12) {//Si le FOV est au dessus de douze.
                                    GameView.this.map.resetFov(GameView.this.map.getFov()-1);
                                    updateScopePanel();
                                }
                                
                            } else {
                                //"Molette tournée vers le bas"
                                // System.out.println("bas");
                                if(GameView.this.map.getFov()<20) //Si le FOV est en dessous de vingt.
                                    GameView.this.map.resetFov(GameView.this.map.getFov()+1);
                                    updateScopePanel();
                            }

                        }
                    });

                }

                    private void updateScopePanel(){
                        
                        int fov = GameView.this.map.getFov();
                        int percentage = (fov-12) * 100 / 8;
                        System.out.println(percentage);
                        int scopeBarHeight = ((GameView)Config.window.getContentPane()).getScopePanel().scopeManage.getHeight();
                        int Ysep = scopeBarHeight * percentage / 100;
                        ScopeIcon sI = ((GameView)Config.window.getContentPane()).getScopePanel().scopeIcon;
                        ScopeManage sM = ((GameView)Config.window.getContentPane()).getScopePanel().scopeManage;
                        sI.setLocation(sI.getX(), Ysep + Config.adaptValueToScreen(80, -1) - sI.getHeight()/2);
                        sM.updateYSep(Ysep);
                    }


                private void initBackground(String backgroundLocation){
                    this.background = new JPanel(){

                        private Image background = new ImageIcon(backgroundLocation).getImage();

                        {

                            this.setBounds(
                                0,
                                Config.adaptValueToScreen(-10000, 1),
                                InGameView.this.getWidth(),
                                InGameView.this.getHeight() + Config.adaptValueToScreen(10000, -1)
                            );
                            this.setOpaque(false);

                        }

                        @Override
                        public void paintComponent(Graphics g){
                            Graphics2D g2d = (Graphics2D)g;
                            g2d.drawImage(background, 0, 0, this.getWidth(), this.getHeight(), null);
                            super.paintComponent(g);
                        }

                    };
                }
                
                /**
                 * Cette fonction lance un Thread qui va faire descendre le background vers le bas en permanence.
                 */
                private void makeThreadBackgroundMoving(){
                    Thread thread = 
                        new Thread(
                            ()->{
                                while(backgroundThreadactive && GameUpdater.isThreadWorking){ //Tant qu'il est possible d'animer l'arrière plan.
                                    try{
                                        updateBackgroundLocation();
                                        Thread.sleep(20);
                                    }catch(InterruptedException e){
                                        System.out.println("Thread d'animation d'arrière plan stoppé !");
                                    }
                                    catch(IllegalComponentStateException e){
                                        //Ce n'est pas grave, cela est due au fait que tous les éléments ne sont pas encore initilisés au début du Thread.
                                    }
                                }
                            },
                            "GameView - Background Moving Thread"
                        );
                        
                    Config.toInterupt.add(thread);
                    thread.start();
                }

                        /**
                         * Fonction qui mets à jour la position de l'arrière plan pour faire l'animation.
                         * @throws IllegalComponentStateException Lorsque tous les éléments ne sont pas encore initilisés au début du Thread.
                         */
                        private void updateBackgroundLocation() throws IllegalComponentStateException{
                            
                            if(GameView.this.getLocationOnScreen().y <= this.background.getLocationOnScreen().y){
                                this.background.setBounds(
                                    0,
                                    this.getY() + Config.adaptValueToScreen(-10000, -1),
                                    this.background.getWidth(),
                                    this.background.getHeight()
                                );
                            }
                            else{
                                this.background.setBounds(
                                    0,
                                    this. background.getY() + this.bacgroundDY,
                                    this.background.getWidth(),
                                    this.background.getHeight()
                                );
                            }

                        }

    
        /**
         * Fonction qui va rafraichir la dimension des éléments à l'écran, si jamais l'écran est redimensionné pendant la 
         * partie
         */
        @Override
        public void refreshGraphics(){
            this.setBounds( //Mise à l'échelle de l'interface de jeu
                Config.adaptValueToScreen(decalageX, 1), 
                Config.adaptValueToScreen(decalageY, -1),
                Config.adaptValueToScreen(this.getWidth(), 1),
                Config.adaptValueToScreen(this.getHeight(), -1)
            );
            this.setLayout(null); //Pas de Layout
        }
        
        /**
         * Fonction qui arrête l'animation de l'arrière plan du jeu.
         */
        public void stopBackgroundAnime(){
            this.backgroundThreadactive = false;
        }

        /**
         * Fonction qui revoie la valeur actuelle du décalage de l'arrière plan.
         * @return Décalage de l'arrière plan.
         */
        public int getBackgroundDY(){
            return this.bacgroundDY;
        }
        
        /**
         * Fonction qui change le décalage de l'arrière plan.
         * @param bacgroundDY Nouveau décalage de l'arrière plan
         */
        public void setBackgroundDY(int bacgroundDY){
            this. bacgroundDY= bacgroundDY;
        }
    }


    /**
     * Fonction qui va rafraichir la dimension des éléments à l'écran, si jamais l'écran est redimensionné pendant la 
     * partie
     */
    @Override
    public void refreshGraphics(){
        this.setBounds(0,0,Config.screenW, Config.screenH); //Remise à l'échelle du GameView.
        this.inGameView.refreshGraphics(); //Remise à l'echelle du JPanel contenant tous les éléments du jeu.
    }
  
    /**
     * Getter de l'interface de jeu
     */
    public InGameView getInGameView(){return this.inGameView;}

    public ArmingInterface getArmingInterface(){
        return this.armingPanel;
    }

    

    /**
     * Fonction qui renvoi le panneau des entités.
     * <p>Va servir lors de l'ajout des entités dans le jeu tels que le joueur ou les ennemis.
     * @return Le panneau des entités dans le jeu.
     */
    public JPanel getEntityPanel(){
        return this.entityPanel;
    }

    /**
     * Fonction qui renvoi le panneau des items du jeu.
     * <p>Va servir lors de l'ajout des items dans le jeu.
     * @return Le panneau des items dans le jeu.
     */
    public JPanel getItemPanel(){
        return this.itemPanel;
    }

    /**
     * Fonction qui renvoi le panneau des murs du jeu.
     * <p>Va servir lors de l'ajout des murs dans le jeu.
     * @return Le panneau des murs dans le jeu.
     */
    public JPanel getWallsPanel(){
        return this.wallsPanel;
    }

    /**
     * Fonction qui renvoi le panneau des effets visuels du jeu.
     * <p>Va servir lors de l'ajout des effets visuels dans  le jeu.
     * @return Le panneau des effets visuels du jeu.
     */
    public JPanel getFxPanel(){
        return this.fxPanel;
    }

    /**
     * Fonction qui renvoi le panneau de l'inventaire du joueur. 
     * @return Le panneau de l'inventaire.
     */
    public BackPackPanel getBackPackPanel(){
        return this.backpackmenu;
    }

    /**
     * Fonction qui renvoie la référence de la carte.
     * @return La référence de la carte.
     */
    public Map getMap(){return this.map;}

    /**
     * Fonction qui renvoie le panneau affichant le facteur d'echelle de la carte.
     * @return Le panneau affichant le facteur d'echelle de la carte.
     */
    public ScopePanel getScopePanel(){return this.scopePanel;}


    /**
     * Fonction qui change la réfénrence du joueur.
     * @param player la nouvelle référence du joueur.
     */
    public void setBackPackMenuPlayer(Player player){
        this.backpackmenu.setPlayer(player);
    }


    /**
     * Fonction qui met à jour la référence de la carte actuelle du jeu.
     * @param map La nouvelle carte.
     */
    public void setMap(Map map){
        this.map = map;
    }


    //Fonctionnalité abandonnée
    // /**
    //  * Fonction qui renvoi le menu des items.
    //  * @return Le menu des items.
    //  * @see LootMenu
    //  */
    // public LootMenu getLootMenuPanel() {
    //     return lootMenuPanel;
    // }
    
    //Fonctionnalité abandonnée
    // /**
    //  * 
    //  * Fonction qui change le menu des item.
    //  * @return Le nouveau menu des items.
    //  * @see LootMenu
    //  */
    // @Deprecated
    // public void setLootMenuPanel(LootMenu lootMenuPanel) {
    //     this.lootMenuPanel = lootMenuPanel;
    // }

}
package gui.gameview;

import java.awt.Color;
import java.awt.Dimension;

///////////////////////////   [==  Star Quest  ==]   ///////////////////////////////
//  [IMPORTANT] Pour une meilleur lisibilité, masquer les pavés de commentaire avec l'IDE, ces commentaires sont surtout reservés à la documentation HTML (Javadoc).
//////////////////////////////////////////////////////////////////////////////////////////////////////////

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.IllegalComponentStateException;
import java.awt.Image;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import javax.swing.ImageIcon;
import javax.swing.JPanel;

import controller.Config;
import controller.GameUpdater;
import gui.Refreshable;
import gui.gameview.backpackpanel.BackPackPanel;
import gui.gameview.scope.ScopeIcon;
import gui.gameview.scope.ScopeManage;
import gui.gameview.scope.ScopePanel;
import gui.gameview.textpopup.TextPopUp;
import model.Player;
import model.map.Map;
import model.memRes.Ressources;

/**
 * Cette classe représente l'interface de jeu à part entière, elle va contenir le HUD (les boutons, la barre de vie, etc), 
 * ainsi que tous les éléments du jeu comme, le joueur, le ennemis,etc
 */
public class GameView extends JPanel implements Refreshable{

    /**
     * l'interface graphique qui va contenir toutes les objet de jeu jeu comme le joueurs, les énnemis, etc
     */
    private InGameView inGameView;

    /**
     * Cet attribut sera utilisé dans un Thread qui va repaindre en permanence le JPanel pour éviter les effets de glich à l'écran.
     */
    public boolean repaint = true;

    /**
     * Panneau donnant des informations sur les armes.
     */
    private ArmingInterface armingPanel = new ArmingInterface();

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les entités tels que les ennemis et le joueur.
     */
    private JPanel entityPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les items du jeu.
     */
    private JPanel itemPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les Murs du jeu.
     */
    private JPanel wallsPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les effets visuels du jeu.
     */
    private JPanel fxPanel;

    /**
     * Il s'agit du menu de l'inventaire du joueur.
     */
    private BackPackPanel backpackmenu;

    /**
     * Il s'agit de la référence à la carte actuel de la partie.
     */
    private Map map;

    /**
     * Il s'agit du panneau affichant la valeur actuelle du facteur d'echelle de la carte.
     */
    private ScopePanel scopePanel = new ScopePanel();
    
    private TextPopUp barrierEnergyPanel;

    ///**
    // * Fonctionalité abandonnée
    // */
    ///**
    // * Paneau affichant le menu des item à ramasser.
    // */
    // private LootMenu lootMenuPanel = new LootMenu();
    
    /**
     * Constructeur de GameView, il prend en argument les dimension de l'interface de jeu (InGameView)
     */
    public GameView(int decalageX, int decalageY, int width, int height, String bakcgroundLocation){
        Config.window.setContentPane(this); //GameView devient le paneau principale de l'interface.
        this.setLayout(null); //GameView n'a pas de Layout.
        this.inGameView = new InGameView(decalageX, decalageY, width, height, bakcgroundLocation); //Création d'une view du jeu, 
        this.add(inGameView); //Ajout dans l'interface de jeu dans l'interface global GameView.
        this.add(this.armingPanel,0);
        Config.elementRefreshables.add(this); //Ajout de l'interface de jeu comme étant un refreshable, dans Config.
        Config.updatable.add(this);
        this.backpackmenu = new BackPackPanel(null);
        this.setBounds(0,0,Config.screenW, Config.screenH);
        this.setPreferredSize(new Dimension(Config.screenW, Config.screenH));

        // new Thread(()->{
        //     while(repaint){
        //         repaint();
        //         try {
        //             Thread.sleep(1);
        //         } catch (InterruptedException e) {
        //         }
        //     }
        // },"GameView - Repainting").start();

        this.initilizeCanvas(width, height);
        /**
         * Fonctionalité abandonnée
         */
        // this.inGameView.add(lootMenuPanel); //Ajout du menu des loot.
        this.add(backpackmenu, 0);

        Integer [] coords = {
            Config.adaptValueToScreen(30, 1),
            Config.screenH - Config.adaptValueToScreen(100, -1)
        };
        this.barrierEnergyPanel = new TextPopUp("Energie : 0", Color.white, 10, Ressources.PATH+"null.png", coords);
        
    }

    public void setDecalageY(int y){
       inGameView.decalageY = y;
       inGameView.refreshGraphics();
    }

    /**
     * Cette fonction va initialiser tous les panneaux du jeu qui vonte servir de calques pour les différents éléments du jeu,  
     * tels que les entités les murs, les effets visuels, etc.
     * <p>Tous ces panneaux seront contenu dans {@link InGameView}, ainsi ils auront les mêmes dimensions que le inGameView.
     * 
     * @param dX Le décalage dans l'axe X du panneau.
     * @param dY Le décalage dans l'axe Y du panneau.
     * @param w La largeur des panneaux.
     * @param h La hauteur des panneaux.
     */
    private void initilizeCanvas(int w, int h){
        initWallsPanel(w, h);
        initItemPanel(w, h);
        initEntityPanel(w, h); //Création de du panneau des entités.
        initFxPanel(w, h);
        initScopePanel();
    }   
            /**
             * Fonction qui initialise le panneau affichant le facteur de zoom de la carte.
             */
            private void initScopePanel(){
                this.scopePanel = new ScopePanel();
                this.add(scopePanel, 0);
                this.add(scopePanel.scopeIcon.textScope, 0);
            }


            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             * 
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initEntityPanel(int w, int h){
                this.entityPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.entityPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.entityPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.entityPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.entityPanel,0); //Ajout du panneau dans l'interface de jeu.
            }
            
            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initItemPanel(int w, int h){
                this.itemPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.itemPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.itemPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.itemPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.itemPanel,0); //Ajout du panneau dans l'interface de jeu.
            }

            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initWallsPanel(int w, int h){
                this.wallsPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.wallsPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.wallsPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.wallsPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.wallsPanel,0); //Ajout du panneau dans l'interface de jeu.
            }

            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initFxPanel(int w, int h){
                this.fxPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.fxPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.fxPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.fxPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.fxPanel,0); //Ajout du panneau dans l'interface de jeu.
            }





    /**
     * C'est cette Objet de l'interface graphique qui va contenir tous les éléments du jeu comme le joueurs, les énnemis.
     */
    public class InGameView extends JPanel implements Refreshable{

        /**
         * Il s'agit ici d'un décalage, vers la gauche du panneau.
         * <p>
         * <strong>Explication :</strong> La carte du jeu sera faite de manière à avoir un peu plus d'éléments en dehors
         *  de l'écran (C'est à dire en gros que certains éléments seront en dehors de l'écran, exprès). Cela permettera 
         * de faire l'implémentation de fonctionnalités très intéressantes dans le jeu, comme par exemple, des effets de 
         * tremblement de l'écran (Shake), de plus en plus grand pour simuler des effet de caméra lors du tir.
         * 
         * Mais également cela permettra de rajouter des fonctionnalités comme les zooms avec des armes de précision, 
         * (sniper), dans lesquels la map deviendra de plus petite et affichera plus d'éléments, selon le type de zoom
         * 
         * Le décalage en X est alors le décalage vers la gauche qui ferra que le joueur verra ce qui est au centre, mais 
         * pas les alentours.
         */
        private int decalageX; 

        /**
         * Il s'agit ici d'un décalage, vers le bas du panneau.
         * <p>
         * <strong>Explication :</strong> La carte du jeu sera faite de manière à avoir un peu plus d'éléments en dehors
         *  de l'écran (C'est à dire en gros que certains éléments seront en dehors de l'écran, exprès). Cela permettera 
         * de faire l'implémentation de fonctionnalités très intéressantes dans le jeu, comme par exemple, des effets de 
         * tremblement de l'écran (Shake), de plus en plus grand pour simuler des effet de caméra lors du tir.
         * 
         * (Le mode sniper ne changera pas la vue de haut en bas mais uniquement de droite à gauche)
         * 
         * Le décalage en Y est alors le décalage vers le bas qui ferra que le joueur verra ce qui est au centre, mais 
         * pas les alentours.
         */
        private int decalageY;

        /**
         * Panneau qui contiendera l'image d'arrière plan du jeu.
         */
        private JPanel background;

        /**
         * Booléen utilisé pour faire fonctionner le Thread qui faitb bouger la carte.
         * <p>Tant qu'il vaut {@code true} le background du jeu sera animé.
         */
        private boolean backgroundThreadactive = true;

        /**
         * Eniter représentant le décalage à ajouter à l'arrière plan pour le décaler en haut ou en bas.
         */
        private int bacgroundDY = 1;

        /**
         * Constructeur de l'interface, il prend en argument les dimension de l'interface
         */
        InGameView(int decalageX, int decalageY, int width, int height, String backgroundLocation){
            this.decalageX = decalageX; //Mise à jour du décalage en X
            this.decalageY = decalageY; //Mise à jour du décalage en Y
            this.setBounds( //Mise à l'échelle de l'interface de jeu
                decalageX,
                decalageY,
                width,
                height
            );
            this.setLayout(null); //Pas de Layout
            this.setFocusable(true);
            this.setOpaque(false); //Activation de la transparence
            // this.setBackground(new Color(200, 111, 33));
            this.initBackground(backgroundLocation);
            this.add(background);
            this.makeThreadBackgroundMoving();
            this.addMouseWheelActions();
            // System.out.println("inGameView : [X = "+decalageX+", Y = "+decalageY+", Width = "+width+", Height = "+height); //debug
        }

                /**
                 * Fonction qui ajoute les actions à faire lorsque molette de la souris est enclenchée.
                 */
                private void addMouseWheelActions(){

                    this.addMouseWheelListener(new MouseWheelListener() {

                        @Override
                        public void mouseWheelMoved(MouseWheelEvent e){
                            //Délai pour éviter que l'intervalle d'exécution de la fonction ne soit trop rapide.
                                // if (System.currentTimeMillis() - delay < 200)
                                //     return;
                                // delay = System.currentTimeMillis();
                            /////////////////////////////////


                            int dir = e.getWheelRotation(); //Direction actuelle de la rotation de la molette.

                            if (dir < 0) { 
                                //"Molette tournée vers le haut"
                                // System.out.println("Haut");
                                if(GameView.this.map.getFov()>12) {//Si le FOV est au dessus de douze.
                                    GameView.this.map.resetFov(GameView.this.map.getFov()-1);
                                    updateScopePanel();
                                }
                                
                            } else {
                                //"Molette tournée vers le bas"
                                // System.out.println("bas");
                                if(GameView.this.map.getFov()<20) //Si le FOV est en dessous de vingt.
                                    GameView.this.map.resetFov(GameView.this.map.getFov()+1);
                                    updateScopePanel();
                            }

                        }
                    });

                }

                    private void updateScopePanel(){
                        
                        int fov = GameView.this.map.getFov();
                        int percentage = (fov-12) * 100 / 8;
                        System.out.println(percentage);
                        int scopeBarHeight = ((GameView)Config.window.getContentPane()).getScopePanel().scopeManage.getHeight();
                        int Ysep = scopeBarHeight * percentage / 100;
                        ScopeIcon sI = ((GameView)Config.window.getContentPane()).getScopePanel().scopeIcon;
                        ScopeManage sM = ((GameView)Config.window.getContentPane()).getScopePanel().scopeManage;
                        sI.setLocation(sI.getX(), Ysep + Config.adaptValueToScreen(80, -1) - sI.getHeight()/2);
                        sM.updateYSep(Ysep);
                    }


                private void initBackground(String backgroundLocation){
                    this.background = new JPanel(){

                        private Image background = new ImageIcon(backgroundLocation).getImage();

                        {

                            this.setBounds(
                                0,
                                Config.adaptValueToScreen(-10000, 1),
                                InGameView.this.getWidth(),
                                InGameView.this.getHeight() + Config.adaptValueToScreen(10000, -1)
                            );
                            this.setOpaque(false);

                        }

                        @Override
                        public void paintComponent(Graphics g){
                            Graphics2D g2d = (Graphics2D)g;
                            g2d.drawImage(background, 0, 0, this.getWidth(), this.getHeight(), null);
                            super.paintComponent(g);
                        }

                    };
                }
                
                /**
                 * Cette fonction lance un Thread qui va faire descendre le background vers le bas en permanence.
                 */
                private void makeThreadBackgroundMoving(){
                    Thread thread = 
                        new Thread(
                            ()->{
                                while(backgroundThreadactive && GameUpdater.isThreadWorking){ //Tant qu'il est possible d'animer l'arrière plan.
                                    try{
                                        updateBackgroundLocation();
                                        Thread.sleep(20);
                                    }catch(InterruptedException e){
                                        System.out.println("Thread d'animation d'arrière plan stoppé !");
                                    }
                                    catch(IllegalComponentStateException e){
                                        //Ce n'est pas grave, cela est due au fait que tous les éléments ne sont pas encore initilisés au début du Thread.
                                    }
                                }
                            },
                            "GameView - Background Moving Thread"
                        );
                        
                    Config.toInterupt.add(thread);
                    thread.start();
                }

                        /**
                         * Fonction qui mets à jour la position de l'arrière plan pour faire l'animation.
                         * @throws IllegalComponentStateException Lorsque tous les éléments ne sont pas encore initilisés au début du Thread.
                         */
                        private void updateBackgroundLocation() throws IllegalComponentStateException{
                            
                            if(GameView.this.getLocationOnScreen().y <= this.background.getLocationOnScreen().y){
                                this.background.setBounds(
                                    0,
                                    this.getY() + Config.adaptValueToScreen(-10000, -1),
                                    this.background.getWidth(),
                                    this.background.getHeight()
                                );
                            }
                            else{
                                this.background.setBounds(
                                    0,
                                    this. background.getY() + this.bacgroundDY,
                                    this.background.getWidth(),
                                    this.background.getHeight()
                                );
                            }

                        }

    
        /**
         * Fonction qui va rafraichir la dimension des éléments à l'écran, si jamais l'écran est redimensionné pendant la 
         * partie
         */
        @Override
        public void refreshGraphics(){
            this.setBounds( //Mise à l'échelle de l'interface de jeu
                Config.adaptValueToScreen(decalageX, 1), 
                Config.adaptValueToScreen(decalageY, -1),
                Config.adaptValueToScreen(this.getWidth(), 1),
                Config.adaptValueToScreen(this.getHeight(), -1)
            );
            this.setLayout(null); //Pas de Layout
        }
        
        /**
         * Fonction qui arrête l'animation de l'arrière plan du jeu.
         */
        public void stopBackgroundAnime(){
            this.backgroundThreadactive = false;
        }

        /**
         * Fonction qui revoie la valeur actuelle du décalage de l'arrière plan.
         * @return Décalage de l'arrière plan.
         */
        public int getBackgroundDY(){
            return this.bacgroundDY;
        }
        
        /**
         * Fonction qui change le décalage de l'arrière plan.
         * @param bacgroundDY Nouveau décalage de l'arrière plan
         */
        public void setBackgroundDY(int bacgroundDY){
            this. bacgroundDY= bacgroundDY;
        }
    }


    /**
     * Fonction qui va rafraichir la dimension des éléments à l'écran, si jamais l'écran est redimensionné pendant la 
     * partie
     */
    @Override
    public void refreshGraphics(){
        this.setBounds(0,0,Config.screenW, Config.screenH); //Remise à l'échelle du GameView.
        this.inGameView.refreshGraphics(); //Remise à l'echelle du JPanel contenant tous les éléments du jeu.
    }
  
    /**
     * Getter de l'interface de jeu
     */
    public InGameView getInGameView(){return this.inGameView;}

    public ArmingInterface getArmingInterface(){
        return this.armingPanel;
    }

    

    /**
     * Fonction qui renvoi le panneau des entités.
     * <p>Va servir lors de l'ajout des entités dans le jeu tels que le joueur ou les ennemis.
     * @return Le panneau des entités dans le jeu.
     */
    public JPanel getEntityPanel(){
        return this.entityPanel;
    }

    /**
     * Fonction qui renvoi le panneau des items du jeu.
     * <p>Va servir lors de l'ajout des items dans le jeu.
     * @return Le panneau des items dans le jeu.
     */
    public JPanel getItemPanel(){
        return this.itemPanel;
    }

    /**
     * Fonction qui renvoi le panneau des murs du jeu.
     * <p>Va servir lors de l'ajout des murs dans le jeu.
     * @return Le panneau des murs dans le jeu.
     */
    public JPanel getWallsPanel(){
        return this.wallsPanel;
    }

    /**
     * Fonction qui renvoi le panneau des effets visuels du jeu.
     * <p>Va servir lors de l'ajout des effets visuels dans  le jeu.
     * @return Le panneau des effets visuels du jeu.
     */
    public JPanel getFxPanel(){
        return this.fxPanel;
    }

    /**
     * Fonction qui renvoi le panneau de l'inventaire du joueur. 
     * @return Le panneau de l'inventaire.
     */
    public BackPackPanel getBackPackPanel(){
        return this.backpackmenu;
    }

    /**
     * Fonction qui renvoie la référence de la carte.
     * @return La référence de la carte.
     */
    public Map getMap(){return this.map;}

    /**
     * Fonction qui renvoie le panneau affichant le facteur d'echelle de la carte.
     * @return Le panneau affichant le facteur d'echelle de la carte.
     */
    public ScopePanel getScopePanel(){return this.scopePanel;}


    /**
     * Fonction qui change la réfénrence du joueur.
     * @param player la nouvelle référence du joueur.
     */
    public void setBackPackMenuPlayer(Player player){
        this.backpackmenu.setPlayer(player);
    }


    /**
     * Fonction qui met à jour la référence de la carte actuelle du jeu.
     * @param map La nouvelle carte.
     */
    public void setMap(Map map){
        this.map = map;
    }


    //Fonctionnalité abandonnée
    // /**
    //  * Fonction qui renvoi le menu des items.
    //  * @return Le menu des items.
    //  * @see LootMenu
    //  */
    // public LootMenu getLootMenuPanel() {
    //     return lootMenuPanel;
    // }
    
    //Fonctionnalité abandonnée
    // /**
    //  * 
    //  * Fonction qui change le menu des item.
    //  * @return Le nouveau menu des items.
    //  * @see LootMenu
    //  */
    // @Deprecated
    // public void setLootMenuPanel(LootMenu lootMenuPanel) {
    //     this.lootMenuPanel = lootMenuPanel;
    // }

}
package gui.gameview;

import java.awt.Color;
import java.awt.Dimension;

///////////////////////////   [==  Star Quest  ==]   ///////////////////////////////
//  [IMPORTANT] Pour une meilleur lisibilité, masquer les pavés de commentaire avec l'IDE, ces commentaires sont surtout reservés à la documentation HTML (Javadoc).
//////////////////////////////////////////////////////////////////////////////////////////////////////////

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.IllegalComponentStateException;
import java.awt.Image;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import javax.swing.ImageIcon;
import javax.swing.JPanel;

import controller.Config;
import controller.GameUpdater;
import gui.Refreshable;
import gui.gameview.backpackpanel.BackPackPanel;
import gui.gameview.scope.ScopeIcon;
import gui.gameview.scope.ScopeManage;
import gui.gameview.scope.ScopePanel;
import gui.gameview.textpopup.TextPopUp;
import model.Player;
import model.map.Map;
import model.memRes.Ressources;

/**
 * Cette classe représente l'interface de jeu à part entière, elle va contenir le HUD (les boutons, la barre de vie, etc), 
 * ainsi que tous les éléments du jeu comme, le joueur, le ennemis,etc
 */
public class GameView extends JPanel implements Refreshable{

    /**
     * l'interface graphique qui va contenir toutes les objet de jeu jeu comme le joueurs, les énnemis, etc
     */
    private InGameView inGameView;

    /**
     * Cet attribut sera utilisé dans un Thread qui va repaindre en permanence le JPanel pour éviter les effets de glich à l'écran.
     */
    public boolean repaint = true;

    /**
     * Panneau donnant des informations sur les armes.
     */
    private ArmingInterface armingPanel = new ArmingInterface();

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les entités tels que les ennemis et le joueur.
     */
    private JPanel entityPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les items du jeu.
     */
    private JPanel itemPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les Murs du jeu.
     */
    private JPanel wallsPanel;

    /**
     * Il s'agit d'un panneau qui sera utilisé comme calque dans le jeu, qui va contenir les effets visuels du jeu.
     */
    private JPanel fxPanel;

    /**
     * Il s'agit du menu de l'inventaire du joueur.
     */
    private BackPackPanel backpackmenu;

    /**
     * Il s'agit de la référence à la carte actuel de la partie.
     */
    private Map map;

    /**
     * Il s'agit du panneau affichant la valeur actuelle du facteur d'echelle de la carte.
     */
    private ScopePanel scopePanel = new ScopePanel();
    
    private TextPopUp barrierEnergyPanel;

    ///**
    // * Fonctionalité abandonnée
    // */
    ///**
    // * Paneau affichant le menu des item à ramasser.
    // */
    // private LootMenu lootMenuPanel = new LootMenu();
    
    /**
     * Constructeur de GameView, il prend en argument les dimension de l'interface de jeu (InGameView)
     */
    public GameView(int decalageX, int decalageY, int width, int height, String bakcgroundLocation){
        Config.window.setContentPane(this); //GameView devient le paneau principale de l'interface.
        this.setLayout(null); //GameView n'a pas de Layout.
        this.inGameView = new InGameView(decalageX, decalageY, width, height, bakcgroundLocation); //Création d'une view du jeu, 
        this.add(inGameView); //Ajout dans l'interface de jeu dans l'interface global GameView.
        this.add(this.armingPanel,0);
        Config.elementRefreshables.add(this); //Ajout de l'interface de jeu comme étant un refreshable, dans Config.
        Config.updatable.add(this);
        this.backpackmenu = new BackPackPanel(null);
        this.setBounds(0,0,Config.screenW, Config.screenH);
        this.setPreferredSize(new Dimension(Config.screenW, Config.screenH));

        // new Thread(()->{
        //     while(repaint){
        //         repaint();
        //         try {
        //             Thread.sleep(1);
        //         } catch (InterruptedException e) {
        //         }
        //     }
        // },"GameView - Repainting").start();

        this.initilizeCanvas(width, height);
        /**
         * Fonctionalité abandonnée
         */
        // this.inGameView.add(lootMenuPanel); //Ajout du menu des loot.
        this.add(backpackmenu, 0);

        Integer [] coords = {
            Config.adaptValueToScreen(30, 1),
            Config.screenH - Config.adaptValueToScreen(100, -1)
        };
        this.barrierEnergyPanel = new TextPopUp("Energie : 0", Color.white, 10, Ressources.PATH+"null.png", coords);
        
    }

    public void setDecalageY(int y){
       inGameView.decalageY = y;
       inGameView.refreshGraphics();
    }

    /**
     * Cette fonction va initialiser tous les panneaux du jeu qui vonte servir de calques pour les différents éléments du jeu,  
     * tels que les entités les murs, les effets visuels, etc.
     * <p>Tous ces panneaux seront contenu dans {@link InGameView}, ainsi ils auront les mêmes dimensions que le inGameView.
     * 
     * @param dX Le décalage dans l'axe X du panneau.
     * @param dY Le décalage dans l'axe Y du panneau.
     * @param w La largeur des panneaux.
     * @param h La hauteur des panneaux.
     */
    private void initilizeCanvas(int w, int h){
        initWallsPanel(w, h);
        initItemPanel(w, h);
        initEntityPanel(w, h); //Création de du panneau des entités.
        initFxPanel(w, h);
        initScopePanel();
    }   
            /**
             * Fonction qui initialise le panneau affichant le facteur de zoom de la carte.
             */
            private void initScopePanel(){
                this.scopePanel = new ScopePanel();
                this.add(scopePanel, 0);
                this.add(scopePanel.scopeIcon.textScope, 0);
            }


            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             * 
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initEntityPanel(int w, int h){
                this.entityPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.entityPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.entityPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.entityPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.entityPanel,0); //Ajout du panneau dans l'interface de jeu.
            }
            
            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initItemPanel(int w, int h){
                this.itemPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.itemPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.itemPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.itemPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.itemPanel,0); //Ajout du panneau dans l'interface de jeu.
            }

            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initWallsPanel(int w, int h){
                this.wallsPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.wallsPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.wallsPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.wallsPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.wallsPanel,0); //Ajout du panneau dans l'interface de jeu.
            }

            /**
             * Fonction qui initilise un JPanel qui sera utilisé comme calque dans le jeu.
             *  
             * @param w La largeur des panneaux.
             * @param h La hauteur des panneaux.
             */
            private void initFxPanel(int w, int h){
                this.fxPanel = new JPanel(); //Création du panneau des entités dans le jeu.
                this.fxPanel.setOpaque(false); //Suppréssion de l'opacité (mise en transparence du JPanel).
                this.fxPanel.setLayout(null); //Suppression de la diposition prédéfinie.
                this.fxPanel.setBounds(0, 0, w, h); //Mise à l'échelle du panneau. (Le panneau possède les même dimension que InGameView)
                this.inGameView.add(this.fxPanel,0); //Ajout du panneau dans l'interface de jeu.
            }





    /**
     * C'est cette Objet de l'interface graphique qui va contenir tous les éléments du jeu comme le joueurs, les énnemis.
     */
    public class InGameView extends JPanel implements Refreshable{

        /**
         * Il s'agit ici d'un décalage, vers la gauche du panneau.
         * <p>
         * <strong>Explication :</strong> La carte du jeu sera faite de manière à avoir un peu plus d'éléments en dehors
         *  de l'écran (C'est à dire en gros que certains éléments seront en dehors de l'écran, exprès). Cela permettera 
         * de faire l'implémentation de fonctionnalités très intéressantes dans le jeu, comme par exemple, des effets de 
         * tremblement de l'écran (Shake), de plus en plus grand pour simuler des effet de caméra lors du tir.
         * 
         * Mais également cela permettra de rajouter des fonctionnalités comme les zooms avec des armes de précision, 
         * (sniper), dans lesquels la map deviendra de plus petite et affichera plus d'éléments, selon le type de zoom
         * 
         * Le décalage en X est alors le décalage vers la gauche qui ferra que le joueur verra ce qui est au centre, mais 
         * pas les alentours.
         */
        private int decalageX; 

        /**
         * Il s'agit ici d'un décalage, vers le bas du panneau.
         * <p>
         * <strong>Explication :</strong> La carte du jeu sera faite de manière à avoir un peu plus d'éléments en dehors
         *  de l'écran (C'est à dire en gros que certains éléments seront en dehors de l'écran, exprès). Cela permettera 
         * de faire l'implémentation de fonctionnalités très intéressantes dans le jeu, comme par exemple, des effets de 
         * tremblement de l'écran (Shake), de plus en plus grand pour simuler des effet de caméra lors du tir.
         * 
         * (Le mode sniper ne changera pas la vue de haut en bas mais uniquement de droite à gauche)
         * 
         * Le décalage en Y est alors le décalage vers le bas qui ferra que le joueur verra ce qui est au centre, mais 
         * pas les alentours.
         */
        private int decalageY;

        /**
         * Panneau qui contiendera l'image d'arrière plan du jeu.
         */
        private JPanel background;

        /**
         * Booléen utilisé pour faire fonctionner le Thread qui faitb bouger la carte.
         * <p>Tant qu'il vaut {@code true} le background du jeu sera animé.
         */
        private boolean backgroundThreadactive = true;

        /**
         * Eniter représentant le décalage à ajouter à l'arrière plan pour le décaler en haut ou en bas.
         */
        private int bacgroundDY = 1;

        /**
         * Constructeur de l'interface, il prend en argument les dimension de l'interface
         */
        InGameView(int decalageX, int decalageY, int width, int height, String backgroundLocation){
            this.decalageX = decalageX; //Mise à jour du décalage en X
            this.decalageY = decalageY; //Mise à jour du décalage en Y
            this.setBounds( //Mise à l'échelle de l'interface de jeu
                decalageX,
                decalageY,
                width,
                height
            );
            this.setLayout(null); //Pas de Layout
            this.setFocusable(true);
            this.setOpaque(false); //Activation de la transparence
            // this.setBackground(new Color(200, 111, 33));
            this.initBackground(backgroundLocation);
            this.add(background);
            this.makeThreadBackgroundMoving();
            this.addMouseWheelActions();
            // System.out.println("inGameView : [X = "+decalageX+", Y = "+decalageY+", Width = "+width+", Height = "+height); //debug
        }

                /**
                 * Fonction qui ajoute les actions à faire lorsque molette de la souris est enclenchée.
                 */
                private void addMouseWheelActions(){

                    this.addMouseWheelListener(new MouseWheelListener() {

                        @Override
                        public void mouseWheelMoved(MouseWheelEvent e){
                            //Délai pour éviter que l'intervalle d'exécution de la fonction ne soit trop rapide.
                                // if (System.currentTimeMillis() - delay < 200)
                                //     return;
                                // delay = System.currentTimeMillis();
                            /////////////////////////////////


                            int dir = e.getWheelRotation(); //Direction actuelle de la rotation de la molette.

                            if (dir < 0) { 
                                //"Molette tournée vers le haut"
                                // System.out.println("Haut");
                                if(GameView.this.map.getFov()>12) {//Si le FOV est au dessus de douze.
                                    GameView.this.map.resetFov(GameView.this.map.getFov()-1);
                                    updateScopePanel();
                                }
                                
                            } else {
                                //"Molette tournée vers le bas"
                                // System.out.println("bas");
                                if(GameView.this.map.getFov()<20) //Si le FOV est en dessous de vingt.
                                    GameView.this.map.resetFov(GameView.this.map.getFov()+1);
                                    updateScopePanel();
                            }

                        }
                    });

                }

                    private void updateScopePanel(){
                        
                        int fov = GameView.this.map.getFov();
                        int percentage = (fov-12) * 100 / 8;
                        System.out.println(percentage);
                        int scopeBarHeight = ((GameView)Config.window.getContentPane()).getScopePanel().scopeManage.getHeight();
                        int Ysep = scopeBarHeight * percentage / 100;
                        ScopeIcon sI = ((GameView)Config.window.getContentPane()).getScopePanel().scopeIcon;
                        ScopeManage sM = ((GameView)Config.window.getContentPane()).getScopePanel().scopeManage;
                        sI.setLocation(sI.getX(), Ysep + Config.adaptValueToScreen(80, -1) - sI.getHeight()/2);
                        sM.updateYSep(Ysep);
                    }


                private void initBackground(String backgroundLocation){
                    this.background = new JPanel(){

                        private Image background = new ImageIcon(backgroundLocation).getImage();

                        {

                            this.setBounds(
                                0,
                                Config.adaptValueToScreen(-10000, 1),
                                InGameView.this.getWidth(),
                                InGameView.this.getHeight() + Config.adaptValueToScreen(10000, -1)
                            );
                            this.setOpaque(false);

                        }

                        @Override
                        public void paintComponent(Graphics g){
                            Graphics2D g2d = (Graphics2D)g;
                            g2d.drawImage(background, 0, 0, this.getWidth(), this.getHeight(), null);
                            super.paintComponent(g);
                        }

                    };
                }
                
                /**
                 * Cette fonction lance un Thread qui va faire descendre le background vers le bas en permanence.
                 */
                private void makeThreadBackgroundMoving(){
                    Thread thread = 
                        new Thread(
                            ()->{
                                while(backgroundThreadactive && GameUpdater.isThreadWorking){ //Tant qu'il est possible d'animer l'arrière plan.
                                    try{
                                        updateBackgroundLocation();
                                        Thread.sleep(20);
                                    }catch(InterruptedException e){
                                        System.out.println("Thread d'animation d'arrière plan stoppé !");
                                    }
                                    catch(IllegalComponentStateException e){
                                        //Ce n'est pas grave, cela est due au fait que tous les éléments ne sont pas encore initilisés au début du Thread.
                                    }
                                }
                            },
                            "GameView - Background Moving Thread"
                        );
                        
                    Config.toInterupt.add(thread);
                    thread.start();
                }

                        /**
                         * Fonction qui mets à jour la position de l'arrière plan pour faire l'animation.
                         * @throws IllegalComponentStateException Lorsque tous les éléments ne sont pas encore initilisés au début du Thread.
                         */
                        private void updateBackgroundLocation() throws IllegalComponentStateException{
                            
                            if(GameView.this.getLocationOnScreen().y <= this.background.getLocationOnScreen().y){
                                this.background.setBounds(
                                    0,
                                    this.getY() + Config.adaptValueToScreen(-10000, -1),
                                    this.background.getWidth(),
                                    this.background.getHeight()
                                );
                            }
                            else{
                                this.background.setBounds(
                                    0,
                                    this. background.getY() + this.bacgroundDY,
                                    this.background.getWidth(),
                                    this.background.getHeight()
                                );
                            }

                        }

    
        /**
         * Fonction qui va rafraichir la dimension des éléments à l'écran, si jamais l'écran est redimensionné pendant la 
         * partie
         */
        @Override
        public void refreshGraphics(){
            this.setBounds( //Mise à l'échelle de l'interface de jeu
                Config.adaptValueToScreen(decalageX, 1), 
                Config.adaptValueToScreen(decalageY, -1),
                Config.adaptValueToScreen(this.getWidth(), 1),
                Config.adaptValueToScreen(this.getHeight(), -1)
            );
            this.setLayout(null); //Pas de Layout
        }
        
        /**
         * Fonction qui arrête l'animation de l'arrière plan du jeu.
         */
        public void stopBackgroundAnime(){
            this.backgroundThreadactive = false;
        }

        /**
         * Fonction qui revoie la valeur actuelle du décalage de l'arrière plan.
         * @return Décalage de l'arrière plan.
         */
        public int getBackgroundDY(){
            return this.bacgroundDY;
        }
        
        /**
         * Fonction qui change le décalage de l'arrière plan.
         * @param bacgroundDY Nouveau décalage de l'arrière plan
         */
        public void setBackgroundDY(int bacgroundDY){
            this. bacgroundDY= bacgroundDY;
        }
    }


    /**
     * Fonction qui va rafraichir la dimension des éléments à l'écran, si jamais l'écran est redimensionné pendant la 
     * partie
     */
    @Override
    public void refreshGraphics(){
        this.setBounds(0,0,Config.screenW, Config.screenH); //Remise à l'échelle du GameView.
        this.inGameView.refreshGraphics(); //Remise à l'echelle du JPanel contenant tous les éléments du jeu.
    }
  
    /**
     * Getter de l'interface de jeu
     */
    public InGameView getInGameView(){return this.inGameView;}

    public ArmingInterface getArmingInterface(){
        return this.armingPanel;
    }

    

    /**
     * Fonction qui renvoi le panneau des entités.
     * <p>Va servir lors de l'ajout des entités dans le jeu tels que le joueur ou les ennemis.
     * @return Le panneau des entités dans le jeu.
     */
    public JPanel getEntityPanel(){
        return this.entityPanel;
    }

    /**
     * Fonction qui renvoi le panneau des items du jeu.
     * <p>Va servir lors de l'ajout des items dans le jeu.
     * @return Le panneau des items dans le jeu.
     */
    public JPanel getItemPanel(){
        return this.itemPanel;
    }

    /**
     * Fonction qui renvoi le panneau des murs du jeu.
     * <p>Va servir lors de l'ajout des murs dans le jeu.
     * @return Le panneau des murs dans le jeu.
     */
    public JPanel getWallsPanel(){
        return this.wallsPanel;
    }

    /**
     * Fonction qui renvoi le panneau des effets visuels du jeu.
     * <p>Va servir lors de l'ajout des effets visuels dans  le jeu.
     * @return Le panneau des effets visuels du jeu.
     */
    public JPanel getFxPanel(){
        return this.fxPanel;
    }

    /**
     * Fonction qui renvoi le panneau de l'inventaire du joueur. 
     * @return Le panneau de l'inventaire.
     */
    public BackPackPanel getBackPackPanel(){
        return this.backpackmenu;
    }

    /**
     * Fonction qui renvoie la référence de la carte.
     * @return La référence de la carte.
     */
    public Map getMap(){return this.map;}

    /**
     * Fonction qui renvoie le panneau affichant le facteur d'echelle de la carte.
     * @return Le panneau affichant le facteur d'echelle de la carte.
     */
    public ScopePanel getScopePanel(){return this.scopePanel;}


    /**
     * Fonction qui change la réfénrence du joueur.
     * @param player la nouvelle référence du joueur.
     */
    public void setBackPackMenuPlayer(Player player){
        this.backpackmenu.setPlayer(player);
    }


    /**
     * Fonction qui met à jour la référence de la carte actuelle du jeu.
     * @param map La nouvelle carte.
     */
    public void setMap(Map map){
        this.map = map;
    }


    //Fonctionnalité abandonnée
    // /**
    //  * Fonction qui renvoi le menu des items.
    //  * @return Le menu des items.
    //  * @see LootMenu
    //  */
    // public LootMenu getLootMenuPanel() {
    //     return lootMenuPanel;
    // }
    
    //Fonctionnalité abandonnée
    // /**
    //  * 
    //  * Fonction qui change le menu des item.
    //  * @return Le nouveau menu des items.
    //  * @see LootMenu
    //  */
    // @Deprecated
    // public void setLootMenuPanel(LootMenu lootMenuPanel) {
    //     this.lootMenuPanel = lootMenuPanel;
    // }

}
