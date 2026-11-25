/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choose_adaptive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeanbarr <jeanbarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 10:49:32 by jeanbarr          #+#    #+#             */
/*   Updated: 2026/11/25 14:32:14 by jeanbarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

t_strategy choose_adaptive(double disorder)
{
    if (disorder < 0.05)
        return (STRAT_SIMPLE);
    if (disorder < 0.5)
        return (STRAT_MEDIUM);
    return (STRAT_COMPLEX);
}