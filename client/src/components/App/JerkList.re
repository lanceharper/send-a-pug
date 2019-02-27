open BsReactNative;

type item = {
  clickedBy: string,
  animation: Animated.Value.t,
  startPosition: int,
  sentAt: string,
};

type state = list(item);

type action =
  | Click(item)
  | Reset;

let component = ReasonReact.reducerComponent("JerkList");

let containerStyle =
  Style.(
    style([
      flex(1.0),
      height(Pt(600.)),
      backgroundColor(String("transparent")),
    ])
  );

let make = (~inboxResponse, _children) => {
  ...component,

  initialState: () => [],

  willReceiveProps: self => {
    [
      {
        clickedBy: inboxResponse##from,
        sentAt: inboxResponse##sentAt,
        animation: Animated.Value.create(0.0),
        startPosition: Js.Math.random_int(100, 400),
      },
      ...self.state,
    ];
  },

  reducer: ((), _state) => ReasonReact.NoUpdate,

  render: self => {
    open Style;

    let heartList =
      List.map(
        ({animation, clickedBy, sentAt, startPosition}) => {
          let height = 600.0;
          let dividedHeight = height /. 6.0;

          let positionInterpolate =
            Animated.Value.interpolate(
              animation,
              ~inputRange=[0.0, height],
              ~outputRange=`float([0.0, height -. 50.0]),
              (),
            );

          let opacityInterpolate =
            Animated.Value.interpolate(
              animation,
              ~inputRange=[0.0, height -. 200.0],
              ~outputRange=`float([1.0, 0.0]),
              (),
            );

          let scaleInterpolate =
            Animated.Value.interpolate(
              animation,
              ~inputRange=[0.0, 15.0, 30.0],
              ~outputRange=`float([0.0, 1.2, 1.0]),
              ~extrapolate=Animated.Interpolation.Clamp,
              (),
            );

          let wobbleInterpolate =
            Animated.Value.interpolate(
              animation,
              ~inputRange=[
                0.0,
                dividedHeight *. 1.0,
                dividedHeight *. 2.0,
                dividedHeight *. 3.0,
                dividedHeight *. 4.0,
                dividedHeight *. 5.0,
                dividedHeight *. 6.0,
              ],
              ~outputRange=
                `float([0.0, 15.0, (-15.0), 15.0, (-15.0), 15.0, (-15.0)]),
              ~extrapolate=Animated.Interpolation.Clamp,
              (),
            );

          let heartStyle =
            style([
              left(Pt(float_of_int(startPosition))),
              Transform.makeAnimated(
                ~translateX=wobbleInterpolate,
                ~translateY=positionInterpolate,
                ~scaleX=scaleInterpolate,
                ~scaleY=scaleInterpolate,
                (),
              ),
              opacity(Animated(opacityInterpolate)),
            ]);

          <Heart
            heartAnimation=animation
            clickedBy
            key=sentAt
            style=heartStyle
            height
          />;
        },
        self.state,
      );
    <View style=containerStyle>
      <View style=StyleSheet.absoluteFill>
        {ReasonReact.array(Array.of_list(heartList))}
      </View>
    </View>;
  },
};