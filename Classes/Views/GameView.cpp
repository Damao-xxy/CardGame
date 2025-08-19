#include "GameView.h"
#include "cocos2d.h"
//#include "cocos2d/ui/CocosGUI.h"
#include "../cocos2d/cocos/ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;

GameView* GameView::create() {
    GameView* view = new (std::nothrow) GameView();
    if (view && view->init()) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

GameView::GameView()
    : _playfieldLayer(nullptr), _stackLayer(nullptr),
      _scoreLabel(nullptr), _movesLabel(nullptr),
      _undoButton(nullptr), _restartButton(nullptr) {}

GameView::~GameView() {
    _cardViews.clear();
}

bool GameView::init() {
    if (!Layer::init()) {
        return false;
    }
    
    // 初始化UI
    initUI();
    initLayout();
    
    return true;
}

void GameView::initUI() {
    // 获取屏幕尺寸
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 创建背景
    auto background = Sprite::create("background.png");
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    addChild(background, 0);
    
    // 创建游戏区图层
    _playfieldLayer = Layer::create();
    addChild(_playfieldLayer, 1);
    
    // 创建牌堆图层
    _stackLayer = Layer::create();
    addChild(_stackLayer, 1);
    
    // 创建分数标签
    _scoreLabel = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 24);
    _scoreLabel->setPosition(Vec2(origin.x + 100, origin.y + visibleSize.height - 30));
    _scoreLabel->setColor(Color3B::WHITE);
    addChild(_scoreLabel, 2);
    
    // 创建移动次数标签
    _movesLabel = Label::createWithTTF("Moves: 0", "fonts/Marker Felt.ttf", 24);
    _movesLabel->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + visibleSize.height - 30));
    _movesLabel->setColor(Color3B::WHITE);
    addChild(_movesLabel, 2);
    
    // 创建撤销按钮
    _undoButton = MenuItemLabel::create(
        Label::createWithTTF("Undo", "fonts/Marker Felt.ttf", 24),
        CC_CALLBACK_1(GameView::onUndoClicked, this)
    );
    _undoButton->setColor(Color3B::WHITE);
    
    // 创建重新开始按钮
    _restartButton = MenuItemLabel::create(
        Label::createWithTTF("Restart", "fonts/Marker Felt.ttf", 24),
        CC_CALLBACK_1(GameView::onRestartClicked, this)
    );
    _restartButton->setColor(Color3B::WHITE);
    
    // 创建菜单
    auto menu = Menu::create(_undoButton, _restartButton, nullptr);
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 30));
    menu->alignItemsHorizontallyWithPadding(50);
    addChild(menu, 2);
}

void GameView::initLayout() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 设置游戏区位置和大小
    _playfieldLayer->setPosition(origin.x, origin.y + 100);
    _playfieldLayer->setContentSize(Size(visibleSize.width, visibleSize.height - 200));
    
    // 设置牌堆位置
    _stackLayer->setPosition(origin.x + 50, origin.y + 50);
}

void GameView::addCardView(CardView* cardView, bool isInPlayfield) {
    if (!cardView) return;
    
    // 存储卡牌视图映射
    _cardViews[cardView->getCardId()] = cardView;
    
    // 设置点击回调
    cardView->setClickCallback(_cardClickCallback);
    
    // 添加到相应的图层
    if (isInPlayfield) {
        _playfieldLayer->addChild(cardView);
    } else {
        _stackLayer->addChild(cardView);
    }
}

void GameView::updateWithModel(const GameModel* model) {
    if (!model) return;
    
    // 更新分数和移动次数
    _scoreLabel->setString(StringUtils::format("Score: %d", model->getScore()));
    _movesLabel->setString(StringUtils::format("Moves: %d", model->getMovesCount()));
    
    // 更新游戏区卡牌
    for (const auto& cardModel : model->getPlayfieldCards()) {
        auto it = _cardViews.find(cardModel->getCardId());
        if (it != _cardViews.end()) {
            it->second->updateWithModel(cardModel.get());
        }
    }
    
    // 更新牌堆卡牌
    for (const auto& cardModel : model->getStackCards()) {
        auto it = _cardViews.find(cardModel->getCardId());
        if (it != _cardViews.end()) {
            it->second->updateWithModel(cardModel.get());
        }
    }
}

void GameView::playMatchAnimation(int cardId1, int cardId2) {
    auto it1 = _cardViews.find(cardId1);
    auto it2 = _cardViews.find(cardId2);
    
    if (it1 != _cardViews.end() && it2 != _cardViews.end()) {
        // 闪烁效果
        auto blink = Blink::create(0.5f, 3);
        // 缩放效果
        auto scale = ScaleTo::create(0.5f, 1.2f);
        // 组合动作
        auto spawn = Spawn::create(blink, scale, nullptr);
        
        it1->second->runAction(spawn->clone());
        it2->second->runAction(spawn);
        
        // 延迟后移除卡牌
        auto delay = DelayTime::create(0.5f);
        auto remove1 = CallFunc::create([this, cardId1]() {
            auto it = _cardViews.find(cardId1);
            if (it != _cardViews.end()) {
                it->second->removeWithAnimation();
                _cardViews.erase(it);
            }
        });
        
        auto remove2 = CallFunc::create([this, cardId2]() {
            auto it = _cardViews.find(cardId2);
            if (it != _cardViews.end()) {
                it->second->removeWithAnimation();
                _cardViews.erase(it);
            }
        });
        
        runAction(Sequence::create(delay, remove1, remove2, nullptr));
    }
}

void GameView::playMoveAnimation(int cardId, float x, float y, bool toPlayfield) {
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end()) {
        // 移动动画
        it->second->moveToPosition(x, y);
        
        // 如果移动到游戏区，翻转卡牌
        if (toPlayfield) {
            auto delay = DelayTime::create(0.3f);
            auto flip = CallFunc::create([this, cardId]() {
                auto it = _cardViews.find(cardId);
                if (it != _cardViews.end()) {
                    it->second->flipCard(true);
                }
            });
            
            it->second->runAction(Sequence::create(delay, flip, nullptr));
        }
        // 如果移动到牌堆，翻转卡牌
        else {
            auto flip = CallFunc::create([this, cardId]() {
                auto it = _cardViews.find(cardId);
                if (it != _cardViews.end()) {
                    it->second->flipCard(false);
                }
            });
            
            it->second->runAction(flip);
        }
        
        // 移动到目标图层
        auto moveToLayer = CallFunc::create([this, cardId, toPlayfield]() {
            auto it = _cardViews.find(cardId);
            if (it != _cardViews.end()) {
                if (toPlayfield) {
                    _playfieldLayer->addChild(it->second);
                } else {
                    _stackLayer->addChild(it->second);
                }
            }
        });
        
        it->second->runAction(Sequence::create(DelayTime::create(0.3f), moveToLayer, nullptr));
    }
}

void GameView::playEnterAnimation() {
    // 游戏区卡牌从上方飞入
    float delay = 0.0f;
    for (const auto& pair : _cardViews) {
        auto cardView = pair.second;
        Vec2 originalPos = cardView->getPosition();
        cardView->setPosition(originalPos.x, getContentSize().height + 100);
        
        auto move = MoveTo::create(0.5f, originalPos);
        auto ease = EaseBackOut::create(move);
        cardView->runAction(Sequence::create(DelayTime::create(delay), ease, nullptr));
        
        delay += 0.05f;
    }
}

void GameView::setCardClickCallback(const std::function<void(int)>& callback) {
    _cardClickCallback = callback;
}

void GameView::setUndoCallback(const std::function<void()>& callback) {
    _undoCallback = callback;
}

void GameView::setRestartCallback(const std::function<void()>& callback) {
    _restartCallback = callback;
}

void GameView::showGameOverDialog(bool isWin) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 创建半透明遮罩
    auto mask = LayerColor::create(Color4B(0, 0, 0, 180), visibleSize.width, visibleSize.height);
    addChild(mask, 10);
    
    // 创建对话框背景
    auto dialog = Sprite::create("dialog_bg.png");
    dialog->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    mask->addChild(dialog);
    
    // 创建标题
    std::string title = isWin ? "Congratulations!" : "Game Over";
    auto titleLabel = Label::createWithTTF(title, "fonts/Marker Felt.ttf", 36);
    titleLabel->setPosition(Vec2(dialog->getContentSize().width / 2, dialog->getContentSize().height - 50));
    titleLabel->setColor(Color3B::YELLOW);
    dialog->addChild(titleLabel);
    
    // 创建消息
    std::string message = isWin ? "You won the game!" : "Try again!";
    auto messageLabel = Label::createWithTTF(message, "fonts/Marker Felt.ttf", 24);
    messageLabel->setPosition(Vec2(dialog->getContentSize().width / 2, dialog->getContentSize().height / 2));
    messageLabel->setColor(Color3B::WHITE);
    dialog->addChild(messageLabel);
    
    // 创建重新开始按钮
//    auto restartBtn = MenuItemLabel::create(
//        Label::createWithTTF("Play Again", "fonts/Marker Felt.ttf", 24),
//        [this](Ref* sender) {
//            if (_restartCallback) {
//                _restartCallback();
//            }
//            sender->getParent()->getParent()->getParent()->removeFromParent();
//        }
//    );
    // 创建重启按钮
    auto restartBtn = MenuItemLabel::create(
        Label::createWithTTF("Play Again", "fonts/Marker Felt.ttf", 24),
        [this](Ref* sender) {
            // 先执行重启回调
            if (_restartCallback) {
                _restartCallback();
            }
            
            // 安全获取需要移除的节点（避免连续调用getParent()）
            // 1. 将sender转换为Node*（MenuItemLabel继承自Node）
            Node* btnNode = dynamic_cast<Node*>(sender);
            if (!btnNode) {
                CCLOG("Error: sender is not a Node");
                return;
            }
            
            // 2. 逐层获取父节点（明确层级关系，提高可读性）
            Node* menuNode = btnNode->getParent();       // 按钮的父节点是Menu
            Node* layerNode = menuNode ? menuNode->getParent() : nullptr;  // Menu的父节点（通常是Layer）
            Node* targetNode = layerNode ? layerNode->getParent() : nullptr;  // 最终要移除的节点
            
            // 3. 安全移除节点
            if (targetNode) {
                targetNode->removeFromParent();
            } else {
                CCLOG("Error: Could not find target node to remove");
            }
        }
    );
    restartBtn->setColor(Color3B::GREEN);
    
    auto menu = Menu::create(restartBtn, nullptr);
    menu->setPosition(Vec2(dialog->getContentSize().width / 2, 50));
    dialog->addChild(menu);
}

void GameView::onUndoClicked(Ref* sender) {
    if (_undoCallback) {
        _undoCallback();
    }
}

void GameView::onRestartClicked(Ref* sender) {
    if (_restartCallback) {
        _restartCallback();
    }
}
