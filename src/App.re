/* Create a GraphQL Query by using the graphql_ppx */
module GetPokemons = [%graphql
  {|
  query {
      pokemons(offset: 0) {
          id
          name
          sprites{
            default{
              front
              back
            }
          }
      }
  }
  |}
];

module GetPokemonsQuery = ReasonApollo.CreateQuery(GetPokemons);

module Style = {
  open Css;

  let pokemonContainer =
    style([display(flexBox), flexDirection(row), flexWrap(wrap)]);

  let pokemonTitle = style([textAlign(center)]);

  let pokemonFrontImage = style([textAlign(center)]);

  let pokemonBackImage = style([display(none)]);

  let pokemonCard =
    style([
      backgroundColor(white),
      boxShadow(~y=px(3), ~blur=px(5), rgba(0, 0, 0, 0.3)),
      marginBottom(px(16)),
      marginRight(px(16)),
      width(px(200)),
      padding(px(6)),
      hover([
        selector("& > " ++ pokemonFrontImage, [display(none)]),
        selector("& > " ++ pokemonBackImage, [display(inlineBlock)]),
      ]),
    ]);
};

[@react.component]
let make = () => {
  <GetPokemonsQuery>
    ...{({result}) =>
      switch (result) {
      | Loading => <div> {ReasonReact.string("Loading")} </div>
      | Error(error) => <div> {ReasonReact.string(error##message)} </div>
      | Data(response) =>
        <>
          <h1> {React.string("My Pokedex")} </h1>
          <div className=Style.pokemonContainer>
            {response##pokemons
             ->Belt.Array.mapWithIndex((i, pokemon) =>
                 <div key={string_of_int(i)} className=Style.pokemonCard>
                   <div className=Style.pokemonFrontImage>
                     <img src={pokemon##sprites##default##front} />
                   </div>
                   <div className=Style.pokemonBackImage>
                     <img src={pokemon##sprites##default##back} />
                   </div>
                   <div className=Style.pokemonTitle>
                     {React.string(pokemon##name)}
                   </div>
                 </div>
               )
             ->React.array}
          </div>
        </>
      }
    }
  </GetPokemonsQuery>;
};