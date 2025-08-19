#include "CardView.h"
#include "cocos2d.h"

using namespace cocos2d;

CardView* CardView::create(int cardId, CardSuit suit, CardRank rank, std::string color) {
    CardView* card = new (std::nothrow) CardView(cardId);
    if (card && card->init(suit, rank, color)) {
        card->autorelease();
        return card;
    }
    CC_SAFE_DELETE(card);
    return nullptr;
}

CardView::CardView(int cardId)
    : _cardId(cardId), _frontSprite(nullptr), _backSprite(nullptr) {}

CardView::~CardView() {
    if (_frontSprite) {
        _frontSprite->removeFromParent();
    }
    if (_backSprite) {
        _backSprite->removeFromParent();
    }
}


void CardView::setClickCallback(const std::function<void(int)>& callback) {
    _clickCallback = callback;
}

void CardView::updateWithModel(const CardModel* model) {
    if (!model) return;
    
    // 更新位置
    setPosition(model->getPositionX(), model->getPositionY());
    
    // 更新状态
    if (model->getState() == CARD_STATE_FACE_UP) {
        _frontSprite->setVisible(true);
        _backSprite->setVisible(false);
    } else if (model->getState() == CARD_STATE_FACE_DOWN) {
        _frontSprite->setVisible(false);
        _backSprite->setVisible(true);
    }
}

void CardView::flipCard(bool faceUp, float duration) {
    // 缩放翻转动画
    auto scaleDown = ScaleTo::create(duration / 2, 0.1f, 1.0f);
    auto scaleUp = ScaleTo::create(duration / 2, 1.0f, 1.0f);
    
    auto callFunc = CallFunc::create([this, faceUp]() {
        _frontSprite->setVisible(faceUp);
        _backSprite->setVisible(!faceUp);
    });
    
    runAction(Sequence::create(scaleDown, callFunc, scaleUp, nullptr));
}

void CardView::moveToPosition(float x, float y, float duration) {
    runAction(MoveTo::create(duration, Vec2(x, y)));
}

void CardView::removeWithAnimation(float duration) {
    auto fadeOut = FadeOut::create(duration);
    auto removeSelf = RemoveSelf::create();
    runAction(Sequence::create(fadeOut, removeSelf, nullptr));
}

bool CardView::onTouchBegan(Touch* touch, Event* event) {
    // 转换触摸坐标到本地坐标系
    Vec2 touchLocation = convertToNodeSpace(touch->getLocation());
    Rect rect = Rect(0, 0, getContentSize().width, getContentSize().height);
    
    // 检查触摸是否在卡牌范围内
    if (rect.containsPoint(touchLocation)) {
        // 轻微缩放表示选中
        runAction(ScaleTo::create(0.1f, 1.1f));
        return true;
    }
    
    return false;
}

void CardView::onTouchEnded(Touch* touch, Event* event) {
    // 恢复缩放
    runAction(ScaleTo::create(0.1f, 1.0f));
    
    // 触发点击回调
    if (_clickCallback) {
        _clickCallback(_cardId);
    }
}

// 修正：不使用Node容器，直接在_frontSprite上叠加子元素
void CardView::setupTextures(CardSuit suit, CardRank rank, std::string color) {
    // 1. 清除旧的子元素（如果存在）
    if (_suitSprite) {
        _suitSprite->removeFromParent();
        _suitSprite = nullptr;
    }
    if (_rankSprite) {
        _rankSprite->removeFromParent();
        _rankSprite = nullptr;
    }

    // 2. 初始化正面主底板（背景）
    if (!_frontSprite) {
        _frontSprite = Sprite::create("res/card_general.png"); // 卡牌正面背景
        _frontSprite->setContentSize(Size(kCardWidth, kCardHeight));
        _frontSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        addChild(_frontSprite);
    } else {
        // 如果已存在，直接更换背景纹理
        _frontSprite->setTexture("res/card_general.png");
        _frontSprite->setContentSize(Size(kCardWidth, kCardHeight));
    }

    // 3. 添加花色图案（作为_frontSprite的子精灵）
    std::string suitPath = StringUtils::format("res/suits/%s.png", getSuitName(suit).c_str());
    _suitSprite = Sprite::create(suitPath);
    if (_suitSprite) {
        _suitSprite->setScale(0.6f); // 缩放花色
        _suitSprite->setPosition(_frontSprite->getContentSize() / 2); // 居中
        _frontSprite->addChild(_suitSprite); // 直接添加到_frontSprite上
    } else {
        CCLOG("花色图片加载失败：%s", suitPath.c_str());
    }

    // 4. 添加数字/字母（作为_frontSprite的子精灵）
    std::string rankStr = getRankName(rank);
    std::string sizePrefix = "big"; // 或 "small"，根据游戏逻辑决定用哪种尺寸
    std::string rankPath = StringUtils::format("res/number/%s_%s_%s.png",
                                               sizePrefix.c_str(),
                                               color.c_str(),
                                               rankStr.c_str());
    _rankSprite = Sprite::create(rankPath);
    if (_rankSprite) {
        _rankSprite->setScale(0.2f); // 缩放数字
        // 左上角定位（相对于_frontSprite的坐标系）
        _rankSprite->setPosition(Vec2(
            10 + _rankSprite->getContentSize().width * 0.2f / 2,
            _frontSprite->getContentSize().height - 10 - _rankSprite->getContentSize().height * 0.2f / 2
        ));
        _frontSprite->addChild(_rankSprite); // 直接添加到_frontSprite上
    } else {
        CCLOG("数字图片加载失败：%s", rankPath.c_str());
    }

    // 5. 确保正面和背面居中
    _frontSprite->setPosition(getContentSize() / 2);
    _backSprite->setPosition(getContentSize() / 2);
}

// 修正init方法（_frontSprite初始化为nullptr，在setupTextures中创建）
bool CardView::init(CardSuit suit, CardRank rank, std::string color) {
    if (!Sprite::init()) {
        return false;
    }

    setContentSize(Size(kCardWidth, kCardHeight));
    setAnchorPoint(Vec2(0.5f, 0.5f));

    // 初始化背面
    _backSprite = Sprite::create("res/card_general.png");
    if (_backSprite) {
        _backSprite->setContentSize(getContentSize());
        _backSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
        addChild(_backSprite);
    } else {
        CCLOG("卡牌背面加载失败！");
        return false;
    }

    // 初始化正面（主底板+子元素）
    _frontSprite = nullptr;
    _suitSprite = nullptr;
    _rankSprite = nullptr;
    setupTextures(suit, rank, color); // 在这一步创建_frontSprite并添加子元素

    // 初始状态：背面朝上
    _frontSprite->setVisible(false);
    _backSprite->setVisible(true);

    // 触摸事件（保持不变）
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(CardView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

    
int CardView::getCardId() const {
    return _cardId;  // 返回存储的 cardId
}
